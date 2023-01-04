#include <engpch.h>

#include "Emitter.h"

#include "Engine/Renderer/Renderer.h"

#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <gl/GL.h>
#include <glad/glad.h>

namespace Engine
{

	Emitter::Emitter()
	{
		particleShader = Shader::Create("../Engine/Assets/Shaders/ParticleShader.glsl");
		//particleShader = Shader::Create("../Engine/Assets/Shaders/PlainShader.glsl");
		m_lifeRemaining = m_lifeTime;
	}

	void Emitter::CreateEmitter(const Engine::EmitterProperties& emitterProps)
	{
		m_Position = emitterProps.position;
		m_lifeTime = emitterProps.lifeTime;
		m_lifeRemaining = m_lifeTime;
	}
	void Emitter::CreateEmitter(const glm::vec3& position, const float& lifetime)
	{
		m_Position = position;
		m_lifeTime = lifetime;
		m_lifeRemaining = m_lifeTime;
	}

	void Emitter::PopulateEmitter(Particle particle, int numOfParticles , const Engine::ParticleProperties& particleProps)
	{
		m_ParticlePool.clear();
		m_ParticlePool.reserve(numOfParticles);
		for (int i = 0; i < numOfParticles; i++)
		{
			Particle temp = particle;
			temp.setParticleProperties(particleProps);
			temp.setStartPosition(m_Position); // setting particle position to emitter position
			
			m_ParticlePool.push_back(temp);
		}
	}

	void Emitter::EmitParticles()
	{
		for (auto& particle : m_ParticlePool)
		{
			if (!particle.getStatus())
				continue;

			// Fade away particles
			float life = particle.getLifeRemaining() / particle.getLifeTime();
			glm::vec4 color = (particle.getEndColor() * (1.0f - life)) + (particle.getStartColor() * life); // a, b c ... a * (1.0 - f) + (b* f)
			color.a = color.a * life;

			float size = (particle.getEndSize() * (1.0f - life)) + (particle.getStartSize() * life);

			// Render

			glm::vec3 pos = particle.getPosition();
			float rot = particle.getRotation();
			ENGINE_LOG_INFO("pos {0},{1},{2}", pos.x, pos.y, pos.z);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
				* glm::rotate(glm::mat4(1.0f), rot, { 0.0f, 0.0f, 1.0f })
				* glm::scale(glm::mat4(1.0f), { size, size, size });

			particleShader->Bind();
			Renderer::SubmitParticle(particleShader, color, particle.getMesh().vertexArray, transform);
		}
	}

	void Emitter::UpdateParticles(const double& dt)
	{
		m_lifeRemaining -= static_cast<float>(dt);
		for (auto& particle : m_ParticlePool)
		{
			if (!particle.getStatus())
				continue;

			if (particle.getLifeRemaining() <= 0.0f)
			{
				particle.setStatus(false);
				continue;
			}

			particle.setLifeRemaining(dt);
			particle.setPosition(dt);
			particle.setRotation(dt);
		}
	}
}
