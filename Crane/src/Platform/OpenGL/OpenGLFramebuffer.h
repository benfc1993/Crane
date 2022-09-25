#pragma once

#include "Crane/Renderer/Framebuffer.h"


namespace Crane
{
    class OpenGLFramebuffer : public Framebuffer
    {
    public:
        OpenGLFramebuffer(const FramebufferSpecification& specification);
        virtual ~OpenGLFramebuffer();

        virtual uint32_t GetColorAttachmentRendererId() const override { return m_ColorAttachment; };
        virtual uint32_t GetDepthAttatchment() const override { return m_DepthAttachment; };

        virtual void Resize(uint32_t width, uint32_t height) override;

        virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; };

        void Invalidate();

        virtual void Bind() override;
        virtual void Unbind() override;
    private:
        uint32_t m_RendererId = 0;
        uint32_t m_ColorAttachment = 0, m_DepthAttachment = 0;
        FramebufferSpecification m_Specification;
    };
}
