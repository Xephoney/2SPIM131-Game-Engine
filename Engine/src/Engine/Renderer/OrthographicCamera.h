#pragma once
#include "glm/glm.hpp"

namespace Engine
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		const glm::mat4& GetViewMatrix() const { return m_view; };
		const glm::mat4& GetProjectionMatrix() const { return m_projection; };
		const glm::mat4& GetViewProj() const { return m_view_projection; }

		void SetPosition(const glm::vec3& position) {  m_position = position; RecalculateViewMatrix();
		}
		const glm::vec3& GetPosition() { return m_position; }

		void SetRotation(const float& rotation) { m_rotation = rotation; RecalculateViewMatrix();
		}
		const float& GetRotation() { return m_rotation; }

	private:
		void RecalculateViewMatrix();

		glm::mat4 m_projection;
		glm::mat4 m_view;
		glm::mat4 m_view_projection;

		glm::vec3 m_position;
		float m_rotation = 0.0f;
	};
}
