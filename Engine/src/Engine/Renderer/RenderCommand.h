#pragma once

#include "RendererAPI.h"

namespace Engine
{
	class RenderCommand
	{
	public:
		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& data)
		{
			s_RendererAPI->DrawIndexed(data);
		}

	private:
		static RendererAPI* s_RendererAPI;
	};
}
