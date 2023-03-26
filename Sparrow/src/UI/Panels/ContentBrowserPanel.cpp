#include "ContentBrowserPanel.h"

#include "Crane/AssetsSystem/Prefab/Prefab.h"
#include "UI/UIComponents/Drawers.h"

#include <imgui/imgui.h>



namespace Crane {

	static std::filesystem::path s_isEditing = "";

	static std::filesystem::path s_AssetPath = "assets";

	static float padding = 16.0f;
	static float thumbnailSize = 80.0f;

	ContentBrowserPanel::ContentBrowserPanel(void* editor, bool isRequired)
		: m_Editor(editor), m_CurrentDirectory(Project::GetActive()->GetAssetsPath()), Panel(isRequired)
	{
		m_DirectoryIcon = Texture2D::Create("Resources/icons/contentBrowser/DirectoryIcon.png");
		m_FileIcon = Texture2D::Create("Resources/icons/contentBrowser/FileIcon.png");
		s_AssetPath = Project::GetActive()->GetAssetsPath();
	}

	void ContentBrowserPanel::OnProjectChanged()
	{
		m_CurrentDirectory = Project::GetActive()->GetAssetsPath();
		s_AssetPath = Project::GetActive()->GetAssetsPath();
	}


	void ContentBrowserPanel::OnImGuiRender()
	{
		std::string name = "ContentBrowserPanel" + std::to_string(m_Index);
		ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoScrollbar);
		ImGui::SliderFloat("Size", &thumbnailSize, 30.0f, 128.0f, "%1.f");

		ImVec2 available = ImGui::GetContentRegionAvail();

		if (ImGui::BeginTable("Browser", 2, 0, available, ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable))
		{
			ImGui::TableSetupColumn("treeC", ImGuiTableColumnFlags_WidthFixed, available.x * 0.25f);
			ImGui::TableSetupColumn("thumbsC", ImGuiTableColumnFlags_WidthFixed, available.x * 0.75f);

			ImGui::TableNextColumn();

			if (ImGui::BeginTable("tree", 1, ImGuiTableFlags_ScrollY))
			{
				ImGui::TableNextColumn();
				DrawDirectoryTree(s_AssetPath);
				ImGui::EndTable();
			}

			ImGui::TableNextColumn();
			DrawDirectoryThumbnails();
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY"))
				{

					UUID uuid = UUID(*(uint64_t*)(payload->Data));

					Prefab::CreatePrefabFile(m_ActiveScene, uuid, m_CurrentDirectory);

					ImGui::EndDragDropTarget();
				}
			}
			ImGui::EndTable();
		}


		ImGui::SetWindowFontScale(1.0f);

		ImGui::End();
	}

	void ContentBrowserPanel::DrawDirectoryTree(std::filesystem::path path)
	{


		for (auto& entry : std::filesystem::directory_iterator(path))
		{
			std::string filename = entry.path().filename().string();
			if (entry.is_directory())
			{

				bool isInPath = m_CurrentDirectory.string().find(entry.path().filename()) != std::string::npos;
				ImGui::SetNextItemOpen(isInPath);
				bool selected = m_CurrentDirectory == entry.path();
				bool isOpened = ImGui::TreeNodeEx(filename.c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth | (selected ? ImGuiTreeNodeFlags_Selected : 0));

				if (ImGui::IsItemClicked())
				{
					m_CurrentDirectory = entry.path();
				}

				if (isOpened)
				{
					DrawDirectoryTree(entry.path());
					ImGui::TreePop();
				}
			}
		}

	}

	void ContentBrowserPanel::DrawDirectoryThumbnails()
	{

		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		if (ImGui::Button("<-"))
		{
			if (m_CurrentDirectory != s_AssetPath)
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		ImGui::SameLine();

		ImGui::Text("%s", m_CurrentDirectory.string().erase(0, s_AssetPath.parent_path().string().length()).c_str());


		if (ImGui::BeginTable("thumbnails", columnCount, ImGuiTableFlags_ScrollY, ImGui::GetContentRegionAvail()))
		{
			ImGui::TableNextColumn();

			if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
			{
				ImGui::OpenPopup("DirOptions");
			}

			if (ImGui::BeginPopup("DirOptions"))
			{
				if (ImGui::MenuItem("New Folder"))
				{
					std::filesystem::create_directory(m_CurrentDirectory / "New Folder");
				}
				ImGui::EndPopup();
			}
			for (auto& entry : std::filesystem::directory_iterator(m_CurrentDirectory))
			{
				const auto& path = entry.path();
				auto relativePath = std::filesystem::relative(path, s_AssetPath);
				std::string pathString = path.string();
				std::string filename = entry.path().filename().string();

				ImGui::PushID(fmt::format("{}-{}", pathString, filename).c_str());
				Ref<Texture2D> icon = entry.is_directory() ? m_DirectoryIcon : m_FileIcon;

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				ImVec2 uv0{ 0, 1 }, uv1{ 1, 0 };
				ImGui::ImageButton("##", (ImTextureID)icon->GetRendererId(), { thumbnailSize, thumbnailSize }, uv0, uv1);
				ImGui::PopStyleColor();

				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					if (entry.is_directory())
					{
						m_CurrentDirectory /= path.filename();
					}
					else
					{
						auto extension = path.extension();
						if (extension == ".scene")
						{
							((EditorLayer*)m_Editor)->LoadScene(path);
						}
						else if (extension == ".cs")
						{
							//TODO: Open vscode file
							// system(fmt::format("code {}", path).c_str());
						}
					}
				}

				if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
				{
					ImGui::OpenPopup("FileOptions");
				}


				if (ImGui::BeginPopup("FileOptions"))
				{
					if (ImGui::MenuItem("Rename"))
					{
						s_isEditing = path;

					}

					if (ImGui::MenuItem("Delete"))
					{
						std::filesystem::remove_all(entry.path());
					}

					ImGui::EndPopup();
				}

				if (ImGui::BeginDragDropSource())
				{
					std::string filePath = relativePath.string();
					ImGui::SetDragDropPayload("CONTENT_BROWSER_FILE", (void*)path.c_str(), path.string().length() + 1);
					ImGui::Text("%s", filename.c_str());
					ImGui::EndDragDropSource();
				}

				const std::string extension = path.extension();

				bool editing = s_isEditing == path;

				Drawers::EditableText(editing, filename.erase(filename.length() - extension.length(), filename.length()), "##", ([&](std::string filename) {
					std::filesystem::rename(path, path.parent_path() / fmt::format("{}{}", filename, extension));
					s_isEditing = "";
				}),
					([&]() {
					s_isEditing = "";
				}));

				ImGui::TableNextColumn();

				ImGui::PopID();
			}
			ImGui::EndTable();
		}
	}
}