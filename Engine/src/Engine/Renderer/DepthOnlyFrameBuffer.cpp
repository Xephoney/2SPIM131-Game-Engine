
#include "engpch.h"
#include "DepthOnlyFrameBuffer.h"

#include "Renderer.h"
#include "Platform/OpenGLDepthOnlyFrameBuffer.h"

namespace Engine
{
	std::shared_ptr<DepthOnlyFramebuffer> DepthOnlyFramebuffer::Create(uint32_t _size)
	{
		
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::none: ENGINE_CORE_ASSERT(false, "we currently only support DepthOnlyFramebuffers with a valid graphics api") return nullptr;
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLDepthOnlyFrameBuffer>(_size);
			case RendererAPI::API::Vulkan: ENGINE_CORE_ASSERT(false, "NOT IMPLEMENTED");
			case RendererAPI::API::DirectX12: ENGINE_CORE_ASSERT(false,"NOT IMPLEMENTED");
		default: 
			return nullptr;
		}
	}
}	
