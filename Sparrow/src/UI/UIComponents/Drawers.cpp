#include "Drawers.h"

#include "Crane/Core/Random.h"

#include <filesystem>

namespace Crane {
	namespace Drawers {
		void TextureDrawer(Ref<Texture2D>& texture, std::filesystem::path assetPath)
		{
			ImGui::Image((ImTextureID)texture.get()->GetRendererId(), ImVec2(100.f, 100.0f));

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_FILE"))
				{
					const std::filesystem::path path = std::filesystem::path((char*)payload->Data);
					const std::string extension = path.extension();
					std::filesystem::path fullAssetPath = std::filesystem::path(assetPath) / path;
					if (extension == ".png" || extension == ".jpg" || extension == ".jpeg")
						texture = Texture2D::Create(fullAssetPath);
					ImGui::EndDragDropTarget();
				}
			}

			ImGui::SameLine();

			if (ImGui::Button("Clear"))
			{
				texture = Texture2D::Create(1, 1);
			}
		}

		void EditableText(bool edit, std::string& text, std::string fieldId, std::function<void(std::string)> callback, std::function<void()> onCancel)
		{
			if (edit)
			{

				// ImGui::SameLine(30.0f);
				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy(buffer, text.c_str());

				std::string id = "##" + (fieldId.empty() ? std::to_string(Random::Float()) : fieldId);
				ImGui::SetKeyboardFocusHere();
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 0.0f));
				if (ImGui::InputText(id.c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_AutoSelectAll))
				{
					auto input = std::string(buffer, sizeof(buffer));
					input.erase(input.find('\0'));
					text = input;
				}

				if ((!ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) || ImGui::IsKeyPressed(ImGuiKey_Enter))
				{
					callback(text);
				}

				if (ImGui::IsKeyPressed(ImGuiKey_Escape))
				{
					onCancel();
				}

				ImGui::PopStyleVar();
			}
			else
			{
				// ImGui::SameLine(40.0f);
				std::string name = text.c_str();
				ImGui::Text("%s", name.c_str());

			}

		}
	}
}