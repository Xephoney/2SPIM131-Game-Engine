#pragma once

#include "RenderCommand.h"

namespace Engine
{
	class Renderer
	{
	public:
		static void NewFrame();

		static void RenderFrame();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); };
	};
}
