#pragma once

#include <vector>

namespace Crane
{

    enum class FrameBufferTextureFormat
    {
        None = 0,

        RGBA8,
        RED_INTEGER,

        DEPTH24STENCIL8,

        Depth = DEPTH24STENCIL8
    };

    struct FrameBufferTextureSpecification
    {
        FrameBufferTextureSpecification() = default;
        FrameBufferTextureSpecification(const FrameBufferTextureFormat format)
            : TextureFormat(format)
        {
        }

        FrameBufferTextureFormat TextureFormat = FrameBufferTextureFormat::None;
    };

    struct FrameBufferAttachmentSpecification
    {
        FrameBufferAttachmentSpecification() = default;
        FrameBufferAttachmentSpecification(std::initializer_list<FrameBufferTextureSpecification> attachments)
            : Attachments(attachments)
        {
        }
        std::vector<FrameBufferTextureSpecification> Attachments;
    };

    struct FramebufferSpecification
    {
        uint32_t Width = 0, Height = 0;
        FrameBufferAttachmentSpecification Attachments;
        uint32_t Samples = 1;

        bool IsSwapChainTarget = false;
        // bool operator==(const FramebufferSpecification& other) { return *this == other; }
    };

    class Framebuffer
    {
    public:
        static Ref<Framebuffer> Create(const FramebufferSpecification& specification);
        virtual ~Framebuffer() {}

        virtual uint32_t GetColorAttachmentRendererId(uint32_t index = 0) const = 0;
        virtual uint32_t GetDepthAttatchment() const = 0;
        virtual void Resize(uint32_t width, uint32_t height) = 0;

        virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;

        virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;

        virtual const FramebufferSpecification& GetSpecification() const = 0;

        virtual void Bind() = 0;
        virtual void Unbind() = 0;
    };
}