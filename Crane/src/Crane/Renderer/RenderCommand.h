#pragma once

#include "RendererAPI.h"



namespace Crane {
    class RenderCommand
    {
    public:
        inline static void Init() { s_RendererAPI->Init(); };

        inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
        {
            s_RendererAPI->SetViewport(x, y, width, height);
        }

        inline static void SetClearColor(const glm::vec4& color) { s_RendererAPI->SetClearColor(color); };
        inline static void Clear() { s_RendererAPI->Clear(); };

        static inline void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0)
        {
            s_RendererAPI->DrawIndexed(vertexArray, indexCount);
        }

        static inline void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
        {
            s_RendererAPI->DrawLines(vertexArray, vertexCount);
        }

        static void SetLineWidth(float lineWidth)
        {
            s_RendererAPI->SetLineWidth(lineWidth);
        }

    private:
        static RendererAPI* s_RendererAPI;
    };
}