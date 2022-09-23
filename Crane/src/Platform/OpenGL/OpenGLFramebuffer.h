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

        virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; };

        void Invalidate();

        virtual void Bind() override;
        virtual void Unbind() override;
    private:
        uint32_t m_RendererId;
        uint32_t m_ColorAttachment, m_DepthAttachment;
        FramebufferSpecification m_Specification;
    };
}
