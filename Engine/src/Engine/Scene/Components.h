#pragma once
#include "glm/glm.hpp"
#include <string>

#include "Engine/Renderer/VertexArray.h"


namespace Engine
{

	struct Transform
	{
		glm::mat4 transform;
		Transform() = default;
		Transform(const Transform&) = default;
		Transform(const glm::mat4& _transform)
			: transform(_transform) { }

		operator glm::mat4& () { return transform; }
		operator const glm::mat4& () const { return transform; }
	};

	struct Camera
	{
		bool orthographic;
		Camera(bool orthographic = false)
			: orthographic(orthographic)
			{}
		
	};

	struct Tag
	{
		std::string tag;

		operator std::string& () { return tag; }
		operator const std::string& () const { return tag; }
	};

	struct Mesh
	{
		std::shared_ptr<VertexArray> vertexArray;

	};
}
