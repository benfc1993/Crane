#pragma once

namespace Crane
{
    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer(){};

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        static VertexBuffer *Create(uint32_t size, float *vertices);
    };

    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer(){};

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual uint32_t GetCount() const = 0;

        static IndexBuffer *Create(uint32_t size, uint32_t *indices);
    };
}