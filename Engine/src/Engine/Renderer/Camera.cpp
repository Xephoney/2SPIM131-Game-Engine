#include <engpch.h>


#include "Camera.h"


#include "glm/glm.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace Engine
{
	OrthographicCamera::OrthographicCamera(const float& left, const float& right, const float& bottom, const float& top)
	{
		m_projection = glm::ortho(left, right, bottom, top, -1.0f, 100.0f);
		RecalculateViewMatrix();
	}

	OrthographicCamera::OrthographicCamera(const float& size, const float& aspectRation, const int& near,
		const int& far)
	{
		m_projection = glm::ortho(size* aspectRation, size* aspectRation, size, size, static_cast<float>(near), static_cast<float>(far));
	}

	

	PerspectiveCamera::PerspectiveCamera(const float& fov, const float& aspectRation, const float& near,
	                                     const float& far)
	{
		m_projection = glm::perspective(fov, aspectRation, near, far);
		RecalculateViewMatrix();
	}

	Camera::Camera()
		: m_position(glm::vec3{ 0.f }), m_rotation(glm::vec3{ 0.f })
	{
		
	}

	void Camera::RecalculateViewMatrix()
	{
		m_transform = glm::translate(glm::mat4{ 1.f }, m_position);
		m_transform = glm::rotate(m_transform, m_rotation.x, glm::vec3(1, 0, 0));
		m_transform = glm::rotate(m_transform, m_rotation.y, glm::vec3(0, 1, 0));
		m_transform = glm::rotate(m_transform, m_rotation.z, glm::vec3(0, 0, 1));

		m_view = glm::inverse(m_transform);
		m_view_projection = m_projection * m_view;
	}
}
