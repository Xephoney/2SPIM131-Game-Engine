#include <engpch.h>

#include "Camera.h"

#include "Engine/Log.h"
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

	OrthographicCamera::OrthographicCamera(const float& size, const float& aspectRation, const int& _near,
		const int& _far)
	{
		m_projection = glm::ortho(size* aspectRation, size* aspectRation, size, size, static_cast<float>(_near), static_cast<float>(_far));
	}

	void OrthographicCamera::Resize(uint32_t width, uint32_t height)
	{
		//m_projection = glm::ortho(size * aspectRation, size * aspectRation, size, size, static_cast<float>(near), static_cast<float>(far));
	}


	PerspectiveCamera::PerspectiveCamera(const float& fov, const float& aspectRatio, const float& __near,
	                                     const float& __far)
	{
		FOV_degrees = fov;
		_near = __near;
		_far = __far;
		m_projection = glm::perspective(glm::radians(fov), aspectRatio, _near, _far);
		RecalculateViewMatrix();
	}

	void PerspectiveCamera::Resize(uint32_t width, uint32_t height)
	{
		float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
		m_projection = glm::perspective(glm::radians(FOV_degrees), aspectRatio, _near, _far);
		RecalculateViewMatrix();
	}

	Camera::Camera()
		: m_position(glm::vec3{ 0, 0, 10}), m_rotation(glm::vec3{ 0.f })
	{
		
	}
	
	void Camera::update(const double& dt)
	{
		if(glm::length(m_movementDir) > 0)
		{
			m_movementSpeed = std::clamp(m_movementSpeed, 0.2f, FLT_MAX);
			m_position += m_movementDir * static_cast<float>(dt) * m_movementSpeed;
			
		}
		RecalculateViewMatrix();
		m_velocity = m_movementDir * m_movementSpeed;
		m_movementDir = glm::vec3{ 0.f };
	}

	glm::vec3 Camera::Forward() 
	{
		return m_direction;
	}

	
	glm::vec3 Camera::Right()
	{
		m_right = normalize(cross(m_direction, { 0,1,0 }));
		return m_right;
	}

	glm::vec3 Camera::Up() 
	{
		m_up = glm::normalize(cross(m_right, m_direction));
		return m_up;
	}

	glm::vec3 Camera::Velocity() const
	{
		return m_velocity;
	}

	glm::vec3 Camera::VelocityNormalized() const
	{
		return normalize(m_velocity);
	}

	float Camera::VelocityUnit() const
	{
		return length(m_velocity);
	}

	void Camera::RecalculateViewMatrix()
	{
		m_transform = translate(glm::mat4{ 1.f }, m_position);
		m_transform = rotate(m_transform, m_rotation.x, glm::vec3(1, 0, 0));
		m_transform = rotate(m_transform, m_rotation.y, glm::vec3(0, 1, 0));
		m_transform = rotate(m_transform, m_rotation.z, glm::vec3(0, 0, 1));
		m_view = lookAt(m_position, m_position + m_direction, { 0,1,0 });

		m_view_projection = m_projection * m_view;
	}
}
