#include <engpch.h>
#include "Buffer.h"

#include "Renderer.h"

#include "VertexArray.h"

#include "Engine/Log.h"
#include "Platform/OpenGLVertexArray.h"

namespace Engine
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::none:
			ENGINE_LOG_WARNING("No Render API selected | index buffer ");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return new OpenGLVertexArray();


		default:
			break;
		}
		return nullptr;
	}

	
}
