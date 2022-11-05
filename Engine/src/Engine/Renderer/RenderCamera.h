#pragma once
#include "glm/glm.hpp"

namespace Engine
{
	class RenderCamera
	{
	public:
		RenderCamera() = default;
		RenderCamera(const glm::mat4& view, const glm::mat4& projection);

		void SetPosition(const glm::vec3& position) { m_Position = position; }
		void SetPosition(const float& x, const float& y, const float& z) { m_Position = {x,y,z}; }
		const glm::vec3& GetPosition() const { return m_Position; }

		void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; }
		void SetRotation(const float& x, const float& y, const float& z) { m_Rotation = { x,y,z }; }
		const glm::vec3& GetRotation() const { return m_Rotation; }

		void SetFOV(const float& newFov) { FOV = newFov; }
		float GetFOV(const float& newFov) { return FOV; }

	private:

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Rotation;

		glm::mat4 m_view;
		glm::mat4 m_projection;
		float FOV;
	};
}
