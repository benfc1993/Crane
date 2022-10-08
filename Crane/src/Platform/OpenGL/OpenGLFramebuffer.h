#pragma once

#include "Crane/Renderer/Framebuffer.h"


namespace Crane
{
    class OpenGLFramebuffer : public Framebuffer
    {
    public:
        OpenGLFramebuffer(const FramebufferSpecification& specification);
        virtual ~OpenGLFramebuffer();

        virtual uint32_t GetColorAttachmentRendererId(u_int32_t index = 0) const override { CR_CORE_ASSERT(index < m_ColorAttachments.size(), "Color Attachment index not valid"); return m_ColorAttachments[index]; };
        virtual uint32_t GetDepthAttatchment() const override { return m_DepthAttachment; };

        virtual void Resize(uint32_t width, uint32_t height) override;

        virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

        virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

        virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; };

        void Invalidate();

        virtual void Bind() override;
        virtual void Unbind() override;

    private:
        uint32_t m_RendererId = 0;
        FramebufferSpecification m_Specification;

        std::vector<FrameBufferTextureSpecification> m_ColorAttachmentSpecs;
        FrameBufferTextureSpecification m_DepthAttachmentSpec = FrameBufferTextureFormat::None;

        std::vector<uint32_t> m_ColorAttachments;
        uint32_t m_DepthAttachment = 0;
    };
}
