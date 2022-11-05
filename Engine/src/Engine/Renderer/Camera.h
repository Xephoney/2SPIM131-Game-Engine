#pragma once
#include "glm/glm.hpp"

namespace Engine
{
	class Camera
	{
	public :
		Camera();
		const glm::mat4& GetViewMatrix() const { return m_view; };
		const glm::mat4& GetProjectionMatrix() const { return m_projection; };
		const glm::mat4& GetViewProj() const { return m_view_projection; }

		void SetPosition(const glm::vec3& position) { m_position = position; RecalculateViewMatrix(); }
		const glm::vec3& GetPosition() const { return m_position; }

		void SetRotation(const glm::vec3& rotation) { m_rotation = rotation; RecalculateViewMatrix(); }
		const glm::vec3& GetRotation() const { return m_rotation; }

	protected:

		void RecalculateViewMatrix();
		glm::mat4 m_projection{ 1.f };
		glm::mat4 m_view{ 1.f };
		glm::mat4 m_view_projection{ 1.f };
		glm::mat4 m_transform{ 1.f };

	private:

		glm::vec3 m_position;
		glm::vec3 m_rotation;

	};

	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(const float& left, const float& right, const float& bottom, const float& top);
		OrthographicCamera(const float& size, const float& aspectRation, const int& near, const int& far);
	private:
		
	};

	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(const float& fov, const float& aspectRation, const float& near, const float& far);
	};
}
