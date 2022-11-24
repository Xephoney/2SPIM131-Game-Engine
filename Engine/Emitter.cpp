#include "engpch.h"
#include "Emitter.h"

Emitter::Emitter()
{

}

void Emitter::CreateEmitter(const Engine::EmitterProperties& emitterProps)
{
	m_Position = emitterProps.position;
	m_lifeTime = emitterProps.lifeTime;
}

void Emitter::PopulateEmitter(Particle& particle, int numOfParticles)
{
	m_ParticlePool.resize(numOfParticles);
	for (int i = 0; i < numOfParticles; i++)
	{
		particle.setStartPosition(m_Position); // setting particle position to emitter position
		m_ParticlePool.push_back(particle);
	}
}

void Emitter::EmitParticles(GLint ParticleShaderViewProj, GLint ParticleShaderTransform, GLint ParticleShaderColor)
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
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { particle.getPosition().x, particle.getPosition().y, 0.0f })
			* glm::rotate(glm::mat4(1.0f), particle.getRotation(), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size, size, 10.0f });
		glUniformMatrix4fv(ParticleShaderTransform, 1, GL_FALSE, glm::value_ptr(transform));
		glUniform4fv(ParticleShaderColor, 1, glm::value_ptr(color));
		glBindVertexArray(particle.m_QuadVA);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
}

void Emitter::UpdateParticles(float ts)
{
	m_lifeRemaining = -ts;
	for (auto& particle : m_ParticlePool)
	{
		if (!particle.getStatus())
			continue;

		if (particle.getLifeRemaining() <= 0.0f)
		{
			particle.setStatus(false);
			continue;
		}

		particle.setLifeRemaining(ts);
		particle.setPosition(ts);
		particle.setRotation(ts);
	}
}