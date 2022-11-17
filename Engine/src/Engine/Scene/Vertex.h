#pragma once
#include "glm/glm.hpp"

namespace Engine
{
	class Vertex
	{
	public:
		Vertex() = default;
		Vertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec4& col, const glm::vec2& uv): position(pos),
			normal(norm), color(col), uv(uv)
		{
		}

		glm::vec3 position;
		glm::vec3 normal;
		glm::vec4 color;
		glm::vec2 uv;

	};
}
