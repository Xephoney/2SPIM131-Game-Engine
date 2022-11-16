#include "engpch.h"

#include "Framebuffer.h"

#include "Renderer.h"
#include "Platform/OpenGLFramebuffer.h"

namespace Engine
{
	std::shared_ptr<Framebuffer> Framebuffer::Create(const FramebufferSpesification& fbs)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::none: ENGINE_CORE_ASSERT(false, "RendererAPI::API::none is not supported") return nullptr;
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLFramebuffer>(fbs);
		}
	}

}
