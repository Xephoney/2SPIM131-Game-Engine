#include "engpch.h"
#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{
	m_EmitterPool.resize(numOfEmitters);
}

void ParticleSystem::Update(float ts) 
{
	for (auto& emitter : m_EmitterPool)
	{
		if (emitter.getLifeRemaining() > 0)
		{
			emitter.UpdateParticles(ts);
		}
	}
}

void ParticleSystem::Render() 
{
	for (auto& emitter : m_EmitterPool)
	{
		emitter.EmitParticles(m_ParticleShaderViewProj, m_ParticleShaderTransform, m_ParticleShaderColor);
	}
}

/* used to initialize a particle and give it properties */
void ParticleSystem::Emit(Emitter& emitter)
{
	m_EmitterPool.push_back(emitter);
}

void ParticleSystem::init(Engine::OrthographicCamera& camera)
{
	m_ParticleShader = std::unique_ptr<Shader>(new Shader("ParticleShader"));
	m_ParticleShaderViewProj = glGetUniformLocation(m_ParticleShader->getProgram(), "u_ViewProj");
	m_ParticleShaderTransform = glGetUniformLocation(m_ParticleShader->getProgram(), "u_Transform");
	m_ParticleShaderColor = glGetUniformLocation(m_ParticleShader->getProgram(), "u_Color");

	glUseProgram(m_ParticleShader->getProgram()); // One shader for now. 
	glUniformMatrix4fv(m_ParticleShaderViewProj, 1, GL_FALSE, glm::value_ptr(camera.GetProjectionMatrix()));
}



/*
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
*/