
#include <engpch.h>

#include "Renderer.h"

namespace Engine
{
	void Renderer::NewFrame()
	{

	}

	void Renderer::RenderFrame()
	{

	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}
