#pragma once

#include <memory>

namespace Engine
{
	struct FramebufferSpesification
	{
		uint32_t width, height;
		uint32_t samples = 1;

		bool swapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;
		virtual FramebufferSpesification& GetSpesification() = 0;
		virtual const FramebufferSpesification& GetSpesification() const = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		static std::shared_ptr<Framebuffer> Create(const FramebufferSpesification& fbs);

		virtual void Resize(uint32_t w, uint32_t h) = 0;
		virtual void ReadPixel(uint32_t x, uint32_t y) = 0;
		virtual uint32_t GetColorAttachment() = 0;
	};
}
