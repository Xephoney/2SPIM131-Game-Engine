#pragma once
#include "glm/glm.hpp"

namespace Engine
{
	class Vertex
	{
	public:
		Vertex() = default;
		Vertex(const Vertex&) = default;
		Vertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec4& col, const glm::vec2& uv): position(pos),
			normal(norm), color(col), uv(uv)
		{
		}
		~Vertex() = default;
		glm::vec3 position {0,0,0};
		glm::vec3 normal{ 0,0,0 };
		glm::vec4 color{ 0,0,0,1};
		glm::vec2 uv{ 0,0 };

	};
}
