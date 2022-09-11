#pragma once

#include "Crane/Renderer/Shader/Buffer.h"

namespace Crane
{
    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(uint32_t size, float* vertices);
        virtual ~OpenGLVertexBuffer();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual const BufferLayout& GetLayout() const override { return m_Layout; };
        virtual void SetLayout(BufferLayout& layout) override { m_Layout = layout; };

    private:
        uint32_t m_RendererId;
        BufferLayout m_Layout;
    };

    class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(uint32_t count, uint32_t* indices);
        virtual ~OpenGLIndexBuffer();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual uint32_t GetCount() const { return m_Count; }

    private:
        uint32_t m_RendererId;
        uint32_t m_Count;
    };

}