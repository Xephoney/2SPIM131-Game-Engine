#pragma once
#include "VertexArray.h"
#include "glm/glm.hpp"
namespace Engine
{
	class RendererAPI
	{
	public:
		enum class API
		{
			none = 0,
			OpenGL = 1,
			DirectX12 = 2,
			Vulkan = 3
		};
	public:
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;

		inline static API GetAPI() { return s_API; }
	private:
		static API s_API;
		
	};
}
