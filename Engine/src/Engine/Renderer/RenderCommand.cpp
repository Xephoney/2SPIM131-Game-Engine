#include "engpch.h"


#include "RenderCommand.h"

#include "Platform/OpenGLRendererAPI.h"

namespace Engine
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
}
