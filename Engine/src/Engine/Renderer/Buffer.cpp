#include "engpch.h"


#include "Buffer.h"

#include "Renderer.h"
#include "Engine/Log.h"
#include "Engine/Scene/Vertex.h"
#include "Platform/OpenGLBuffer.h"

namespace Engine
{
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::none :
			ENGINE_LOG_WARNING("No Render API selected | index buffer ");
			return nullptr;

		case RendererAPI::API::OpenGL :
			return new OpenGLVertexBuffer(vertices, size);

		
		default:
			break;
		}
		return nullptr;
	}


	VertexBuffer* VertexBuffer::Create(const std::vector<Vertex>& verts, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::none:
			ENGINE_LOG_WARNING("No Render API selected | index buffer ");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return new OpenGLVertexBuffer(verts, size);


		default:
			break;
		}
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::none:
			ENGINE_LOG_WARNING("No Render API selected | index buffer");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return new OpenGLIndexBuffer(indices, count);

		default:
			break;
		}
		return nullptr;
	}
}
