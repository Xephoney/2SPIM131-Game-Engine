#pragma once
#include <memory>

namespace Engine
{
	class DepthOnlyFramebuffer
	{
	public:
		virtual ~DepthOnlyFramebuffer() = default;
		static std::shared_ptr<DepthOnlyFramebuffer> Create(uint32_t size);
		virtual void Bind() = 0;
		virtual void BindDepthTexture() = 0;
		virtual void Unbind() = 0;
		[[nodiscard]] virtual uint32_t TextureID() const = 0;
		[[nodiscard]] virtual std::pair<uint32_t,uint32_t> TextureSize() const = 0;
	};

	
}
