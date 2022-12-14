#pragma once

#include "RendererAPI.h"

namespace Engine
{
	class RenderCommand
	{
	public:
		inline static void Init()
		{
			s_RendererAPI->Init();
		}
		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

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

		inline static void DrawIndexedQuad(const std::shared_ptr<VertexArray>& data)
		{
			s_RendererAPI->DrawIndexedQuad(data);
		}

		inline static void DrawLinesIndexed(const std::shared_ptr<VertexArray>& data)
		{
			s_RendererAPI->DrawLinesIndexed(data);
		}

	private:
		static RendererAPI* s_RendererAPI;
	};
}
