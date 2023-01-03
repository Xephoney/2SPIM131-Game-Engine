#pragma once

#include "Engine/Renderer/DepthOnlyFrameBuffer.h"
#include <cstdint>

namespace Engine
{
	class OpenGLDepthOnlyFrameBuffer : public DepthOnlyFramebuffer
	{
	public:
		OpenGLDepthOnlyFrameBuffer(int bufferSize);
	private:
		void Bind() override;
		void Unbind() override;
	public:
		~OpenGLDepthOnlyFrameBuffer() override ;
	private:
		uint32_t TextureID() const override { return depthTexture; }
	public:
		[[nodiscard]] std::pair<uint32_t, uint32_t> TextureSize() const override { return std::pair<uint32_t, uint32_t>(size, size); }
		void BindDepthTexture() override;;
	private:
		uint32_t depthTexture { };
		uint32_t FramebufferName { };
		int size;
	};
}
