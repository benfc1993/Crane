#pragma once

namespace Crane
{

    struct FramebufferSpecification
    {
        uint32_t Width, Height;
        uint32_t Samples = 1;

        bool IsSwapChainTarget = false;
    };

    class Framebuffer
    {
    public:
        static Ref<Framebuffer> Create(const FramebufferSpecification& specification);
        virtual ~Framebuffer() {}

        virtual uint32_t GetColorAttachmentRendererId() const = 0;

        virtual const FramebufferSpecification& GetSpecification() const = 0;

        virtual void Bind() = 0;
        virtual void Unbind() = 0;
    };
}