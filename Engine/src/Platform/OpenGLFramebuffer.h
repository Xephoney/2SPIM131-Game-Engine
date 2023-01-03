#pragma once
#include "Engine/Core.h"
#include "Engine/Log.h"
#include "Engine/Renderer/Framebuffer.h"

 
namespace Engine
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const Engine::FramebufferSpesification& spec);
		virtual ~OpenGLFramebuffer() override;

		FramebufferSpesification& GetSpesification() override { return spesification; }
		const FramebufferSpesification& GetSpesification() const override { return spesification; }

		uint32_t GetColorAttachment(uint32_t index = 0) const override { ENGINE_CORE_ASSERT(index < m_ColorAttachments.size(), "ColorAttachmentIndex is invalid") return m_ColorAttachments[index]; }
		uint32_t GetDepthAttachment() const override { return m_DepthAttachment; }
		void Invalidate();

		virtual void Bind() override ;
		virtual void Unbind() override ;
		void Resize(uint32_t w, uint32_t h) override;
		int ReadPixel(uint32_t attachmentIndex, uint32_t x, uint32_t y) override;
		uint32_t m_DepthAttachment{};
	private:

		uint32_t m_RendererID = 0;
		FramebufferSpesification spesification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecs{};
		FramebufferTextureSpecification m_DepthAttachmentSpec;

		std::vector<uint32_t> m_ColorAttachments {0,0};
		
	};
}
