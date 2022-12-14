#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <memory>

#include "Engine/Random.h"
#include "glm/ext/matrix_transform.hpp"
#include "Engine/Scene/Components.h"

namespace Engine
{
	class Particle
	{
	public:

		Particle();

		void init();
		void setParticleProperties(const Engine::ParticleProperties& particleProps);

		glm::vec3 getPosition() { return m_Position; };
		void setPosition(float ts) { m_Position += m_Velocity * ts; };
		void setStartPosition(glm::vec3 position) { m_Position = position; };

		glm::vec2 getVelocity() const { return m_Velocity; };
		glm::vec4 getStartColor() const { return m_StartColor; };
		glm::vec4 getEndColor() const { return m_EndColor; };

		float getRotation() const { return m_Rotation; };
		void setRotation(float ts) { m_Rotation += 2.f * ts; };

		float getStartSize() const { return m_StartSize; };
		float getEndSize() const { return m_EndSize; };
		float getLifeTime() const { return m_LifeTime; };

		float getLifeRemaining() const { return m_LifeRemaining; };
		void setLifeRemaining(float ts) { m_LifeRemaining -= ts; };

		bool getStatus() const { return m_Active; };
		void setStatus(bool status) { m_Active = status; };

		StaticMesh mesh;
		[[nodiscard]] Mesh getMesh() const { return MeshManager::instance().GetMeshFromID(mesh.meshes.front()); }
				
	private:
		/* Main particle variables */
		glm::vec3 m_Position;
		glm::vec3 m_Velocity;
		glm::vec4 m_StartColor, m_EndColor;
		float m_Rotation = 0.0f;
		float m_StartSize{ 1 }, m_EndSize {1.0};

		float m_LifeTime = 5.0f;
		float m_LifeRemaining = 0.0f;

		bool m_Active = true;

		

		/* End of main particle variables */
	};
}
