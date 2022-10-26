#include "engpch.h"


#include "Buffer.h"

#include "Renderer.h"
#include "Engine/Log.h"
#include "Platform/OpenGLBuffer.h"

namespace Engine
{
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::none :
			ENGINE_LOG_WARNING("No Render API selected | index buffer ");
			return nullptr;

		case RenderAPI::OpenGL :
			return new OpenGLVertexBuffer(vertices, size);

		
		default:
			break;
		}
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::none:
			ENGINE_LOG_WARNING("No Render API selected | index buffer");
			return nullptr;

		case RenderAPI::OpenGL:
			return new OpenGLIndexBuffer(indices, count);

		default:
			break;
		}
		return nullptr;
	}
}
