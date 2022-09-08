#pragma once

#include "RendererAPI.h"



namespace Crane {
    class RenderCommand
    {
    public:
        inline static void Init() { s_RendererAPI->Init(); };
        inline static void SetClearColor(const glm::vec4& color) { s_RendererAPI->SetClearColor(color); };
        inline static void Clear() { s_RendererAPI->Clear(); };

        static inline void DrawIndexed(const Ref<VertexArray>& vertexArray)
        {
            s_RendererAPI->DrawIndexed(vertexArray);
        }
    private:
        static RendererAPI* s_RendererAPI;
    };
}