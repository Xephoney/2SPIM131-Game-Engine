#include "engpch.h"
#include "Particle.h"

Particle::Particle()
{

}

void Particle::init()
{
	if (!m_QuadVA)
	{
		float vertices[] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f, 0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f
		};

		glCreateVertexArrays(1, &m_QuadVA);
		glBindVertexArray(m_QuadVA);

		GLuint quadVB, quadIB;
		glCreateBuffers(1, &quadVB);
		glBindBuffer(GL_ARRAY_BUFFER, quadVB);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexArrayAttrib(quadVB, 0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

		uint32_t indices[] = {
			0, 1, 2, 2, 3, 0
		};

		glCreateBuffers(1, &quadIB);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIB);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	}
}

void Particle::setParticleProperties(const Engine::ParticleProperties& particleProps)
{
	m_Active = true;
	m_Position = particleProps.Position;
	m_Rotation = Random::Float() * 2.0f * 3.14159;

	// Velocity
	m_Velocity = particleProps.Velocity;
	m_Velocity.x += particleProps.VelocityVariation.x * (Random::Float() - 0.5f);
	m_Velocity.y += particleProps.VelocityVariation.y * (Random::Float() - 0.5f);

	// Color
	m_StartColor = particleProps.ColorBegin;
	m_EndColor = particleProps.ColorEnd;

	m_LifeTime = particleProps.LifeTime;
	m_LifeRemaining = particleProps.LifeTime;
	m_StartSize = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Float() - 0.5f);
	m_EndSize = particleProps.SizeEnd;
}
