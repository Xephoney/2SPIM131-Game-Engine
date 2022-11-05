#include <engpch.h>


#include "OrthographicCamera.h"


#include "glm/glm.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace Engine
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
	{
		m_projection = glm::ortho(left, right, bottom, top, -1.0f, 100.0f);
		RecalculateViewMatrix();
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 Transform = glm::translate(glm::mat4{ 1.f }, m_position) * glm::rotate(glm::mat4{ 1.f }, m_rotation, glm::vec3{ 0,0,1 });

		m_view = glm::inverse(Transform);
		m_view_projection = m_projection * m_view;
	}
}
