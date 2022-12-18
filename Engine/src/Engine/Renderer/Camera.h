#pragma once
#include "glm/glm.hpp"

namespace Engine
{
	class Camera
	{
	public :
		virtual ~Camera() = default;
		Camera();
		const glm::mat4& GetViewMatrix() const { return m_view; };
		const glm::mat4& GetProjectionMatrix() const { return m_projection; };
		const glm::mat4& GetViewProj() const { return m_view_projection; }

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		void SetPosition(const glm::vec3& position) { m_position = position; RecalculateViewMatrix(); }
		const glm::vec3& GetPosition() const { return m_position; }

		void SetRotation(const glm::vec3& rotation) { m_rotation = rotation; RecalculateViewMatrix(); }
		const glm::vec3& GetRotation() const { return m_rotation; }

		virtual void update(const double& dt);

		glm::vec3& Direction() { return m_direction; }
		glm::vec3 Forward();
		glm::vec3 Right();
		glm::vec3 Up();

		glm::vec3 m_movementDir{ 0, 0, 0 };
		float m_movementSpeed = 10.f;
	protected:
		void RecalculateViewMatrix();
		glm::mat4 m_projection{ 1.f };
		glm::mat4 m_view{ 1.f };
		glm::mat4 m_view_projection{ 1.f };
		glm::mat4 m_transform{ 1.f };

		glm::vec3 m_position	{ 0, 0, 10 };
		glm::vec3 m_rotation	{ 0, 0, 0 };
		glm::vec3 m_direction	{ 0, 0,-1 };
		glm::vec3 m_right		{ 1, 0,0 };
		glm::vec3 m_up			{ 0, 1,0 };

	private:
	};

	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(const float& left, const float& right, const float& bottom, const float& top);
		OrthographicCamera(const float& size, const float& aspectRation, const int& near, const int& far);
		void Resize(uint32_t width, uint32_t height) override;
	private:
		
	};

	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(const float& fov, const float& aspectRation, const float& near_plane, const float& far_plane);
		void Resize(uint32_t width, uint32_t height) override;
	private:
		float FOV_degrees;
		float _near;
		float _far;
	};
}
