#include "crpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>



namespace Crane
{

    static GLenum ShaderDataTypeToOpenGLDataType(ShaderDataType type)
    {
        switch (type)
        {
        case Crane::ShaderDataType::Float:      return GL_FLOAT;
        case Crane::ShaderDataType::Float2:     return GL_FLOAT;
        case Crane::ShaderDataType::Float3:     return GL_FLOAT;
        case Crane::ShaderDataType::Float4:     return GL_FLOAT;
        case Crane::ShaderDataType::Mat3:       return GL_FLOAT;
        case Crane::ShaderDataType::Mat4:       return GL_FLOAT;
        case Crane::ShaderDataType::Int:        return GL_INT;
        case Crane::ShaderDataType::Int2:       return GL_INT;
        case Crane::ShaderDataType::Int3:       return GL_INT;
        case Crane::ShaderDataType::Int4:       return GL_INT;
        case Crane::ShaderDataType::Bool:       return GL_BOOL;
        }

        CR_CORE_ASSERT(false, "Invalid shader data type!");
        return 0;
    }

    OpenGLVertexArray::OpenGLVertexArray()
    {
        CR_PROFILE_FUNCTION();

        glCreateVertexArrays(1, &m_RendererId);
    }


    OpenGLVertexArray::~OpenGLVertexArray()
    {
        CR_PROFILE_FUNCTION();

        glDeleteVertexArrays(1, &m_RendererId);
    }

    void OpenGLVertexArray::Bind() const
    {
        glBindVertexArray(m_RendererId);
    }

    void OpenGLVertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
    {
        CR_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "VertexBuffer has no layout");

        glBindVertexArray(m_RendererId);
        vertexBuffer->Bind();


        auto layout = vertexBuffer->GetLayout();
        uint32_t index = 0;
        for (const auto& element : layout)
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, element.GetComponentCount(), ShaderDataTypeToOpenGLDataType(element.Type),
                element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)element.Offset);
            index++;
        }
        m_VertexBuffers.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
    {
        glBindVertexArray(m_RendererId);
        indexBuffer->Bind();

        m_IndexBuffer = indexBuffer;
    }
}