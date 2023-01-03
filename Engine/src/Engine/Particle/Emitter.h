#pragma once

#include <glm/glm.hpp>

#include <vector>

#include "glm/ext/matrix_transform.hpp"
#include "Engine/Scene/Components.h"
#include "Particle.h"

namespace Engine
{
	class Emitter
	{
	public:
		Emitter();
		void CreateEmitter(const Engine::EmitterProperties& emitterProps);
		void CreateEmitter(const glm::vec3& position, const float& lifetime);
		void PopulateEmitter(Particle particle, int numOfParticles);
		void EmitParticles();
		void UpdateParticles(const double& dt);
		void setLifeTime(float lifeTime) { m_lifeTime = lifeTime; };
		[[nodiscard]] float getLifeRemaining() const { return m_lifeRemaining; };

	private:
		glm::vec3 m_Position;
		float m_lifeTime = 10.f;
		float m_lifeRemaining = 0;

		std::vector<Particle> m_ParticlePool;
		int numberOfParticles;
		std::shared_ptr<Shader> particleShader;
	};
}
