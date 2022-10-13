#include "Drawers.h"

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
    }
}