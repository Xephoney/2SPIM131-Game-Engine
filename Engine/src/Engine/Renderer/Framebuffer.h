#pragma once

#include <memory>
#include <vector>
namespace Engine
{

	enum class FramebufferTextureFormat
	{
		None = 0,
		RGBA8 = 1,
		RGBA16F = 2,

		DEPTH24STENCIL8,
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			: TextureFormat(format) { }

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;

	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(const std::initializer_list<FramebufferTextureSpecification>& attachments)
			: Attachments(attachments) {}

		std::vector<FramebufferTextureSpecification> Attachments;
	};


	struct FramebufferSpesification
	{
		uint32_t width, height;
		uint32_t samples = 1;
		FramebufferAttachmentSpecification attachments;
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
		[[nodiscard]] virtual uint32_t GetColorAttachment(uint32_t index = 0) const = 0;
	};
}
