#pragma once
#include "Engine/Renderer/Framebuffer.h"

 
namespace Engine
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const Engine::FramebufferSpesification& spec) : spesification(spec) { Invalidate(); }
		virtual ~OpenGLFramebuffer() override;

		FramebufferSpesification& GetSpesification() override { return spesification; }
		const FramebufferSpesification& GetSpesification() const override { return spesification; }

		uint32_t GetColorAttachment() override { return m_ColorAttachment; }

		void Invalidate();

		virtual void Bind() override ;
		virtual void Unbind() override ;
		void Resize(uint32_t w, uint32_t h) override;
		void ReadPixel(uint32_t x, uint32_t y) override;
	private:

		uint32_t m_RendererID = 0;
		uint32_t m_ColorAttachment, m_DepthAttachment;
		FramebufferSpesification spesification;
	};
}
