#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <gl/GL.h>
#include <glad/glad.h>
#include <vector>
#include <memory>

#include "Engine/Renderer/Camera.h"
#include "Engine/Renderer/Shader.h"
#include "Random.h"
#include "glm/ext/matrix_transform.hpp"
#include "Engine/Scene/Components.h"
#include "Particle.h"

class Emitter
{
public:
	Emitter();
	void CreateEmitter(const Engine::EmitterProperties& emitterProps);
	void PopulateEmitter(Particle& particle, int numOfParticles);
	void EmitParticles(GLint m_ParticleShaderViewProj, GLint m_ParticleShaderTransform, GLint m_ParticleShaderColor);
	void UpdateParticles(float ts);
	void setLifeTime(float lifeTime) { m_lifeTime = lifeTime; };
	float getLifeRemaining() { return m_lifeRemaining; };
	void UpdateEmitterPosition(glm::vec2 position) { m_Position = position; };

private:
	glm::vec2 m_Position;
	float m_lifeTime = 10.f;
	float m_lifeRemaining = m_lifeTime;

	std::vector<Particle> m_ParticlePool;
	int numberOfParticles;
};

