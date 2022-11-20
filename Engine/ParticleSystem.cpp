#include "engpch.h"
#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{
	m_ParticlePool.resize(1000);
}

void ParticleSystem::OnUpdate(float ts) // @Param GLCore::Timestep ts
{
	for (auto& particle : m_ParticlePool)
	{
		if (!particle.Active)
			continue;

		if (particle.LifeRemaining <= 0.0f)
		{
			particle.Active = false;
			continue;
		}

		particle.LifeRemaining -= ts;
		particle.Position += particle.Velocity * ts;
		particle.Rotation += 0.01f * ts;
	}
}

void ParticleSystem::OnRender(Engine::Camera& camera) // @Param GLCore::Utils::OrthographicCamera& camera
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

		m_ParticleShader = std::unique_ptr<Shader>(new Shader("ParticleShader"));
		m_ParticleShaderViewProj = glGetUniformLocation(m_ParticleShader->getProgram(), "u_ViewProj");
		m_ParticleShaderTransform = glGetUniformLocation(m_ParticleShader->getProgram(), "u_Transform");
		m_ParticleShaderColor = glGetUniformLocation(m_ParticleShader->getProgram(), "u_Color");
	}

	glUseProgram(m_ParticleShader->getProgram());
	glUniformMatrix4fv(m_ParticleShaderViewProj, 1, GL_FALSE, glm::value_ptr(camera.GetProjectionMatrix()));

	for (auto& particle : m_ParticlePool)
	{
		if (!particle.Active)
			continue;

		// Fade away particles
		float life = particle.LifeRemaining / particle.LifeTime;
		glm::vec4 color = (particle.ColorEnd * (1.0f - life)) + (particle.ColorEnd * life);// a, b c ... a * (1.0 - f) + (b* f)
		color.a = color.a * life;

		float size = (particle.SizeEnd * (1.0f - life)) + (particle.SizeBegin * life);

		// Render
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { particle.Position.x, particle.Position.y, 0.0f })
			* glm::rotate(glm::mat4(1.0f), particle.Rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size, size, 10.0f });
		glUniformMatrix4fv(m_ParticleShaderTransform, 1, GL_FALSE, glm::value_ptr(transform));
		glUniform4fv(m_ParticleShaderColor, 1, glm::value_ptr(color));
		glBindVertexArray(m_QuadVA);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
}

void ParticleSystem::Emit(const ParticleProps& particleProps)
{
	Particle& particle = m_ParticlePool[m_PoolIndex];
	particle.Active = true;
	particle.Position = particleProps.Position;
	particle.Rotation = Random::Float() * 2.0f * 3.14159;

	// Velocity
	particle.Velocity = particleProps.Velocity;
	particle.Velocity.x += particleProps.VelocityVariation.x * (Random::Float() - 0.5f);
	particle.Velocity.y += particleProps.VelocityVariation.y * (Random::Float() - 0.5f);

	// Color
	particle.ColorBegin = particleProps.ColorBegin;
	particle.ColorEnd = particleProps.ColorEnd;

	particle.LifeTime = particleProps.LifeTime;
	particle.LifeRemaining = particleProps.LifeTime;
	particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Float() - 0.5f);
	particle.SizeEnd = particleProps.SizeEnd;

	m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
}



/*
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
*/