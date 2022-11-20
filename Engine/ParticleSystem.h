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


struct ParticleProps
{
	glm::vec2 Position;
	glm::vec2 Velocity, VelocityVariation;
	glm::vec4 ColorBegin, ColorEnd;
	float SizeBegin, SizeEnd, SizeVariation;
	float LifeTime = 1.0f;
};

class ParticleSystem
{
	ParticleSystem();

	void OnUpdate(float ts); // @Param GLCore::Timestep ts
	void OnRender(Engine::Camera& camera); // @Param GLCore::Utils::OrthographicCamera& camera

	void Emit(const ParticleProps& particleProps); 

private:
	struct Particle
	{
		glm::vec2 Position;
		glm::vec2 Velocity;
		glm::vec4 ColorBegin, ColorEnd;
		float Rotation = 0.0f;
		float SizeBegin, SizeEnd;

		float LifeTime = 1.0f;
		float LifeRemaining = 0.0f;

		bool Active = false;
	};
	std::vector<Particle> m_ParticlePool;
	uint32_t m_PoolIndex = 999;

	GLuint m_QuadVA = 0;
	std::unique_ptr<Shader> m_ParticleShader;
	GLint m_ParticleShaderViewProj, m_ParticleShaderTransform, m_ParticleShaderColor;
};

