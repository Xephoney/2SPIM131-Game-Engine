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

class Particle
{
public:

	Particle();

	void init();
	void setParticleProperties(const Engine::ParticleProperties& particleProps);

	glm::vec2 getPosition() { return m_Position; };
	void setPosition(float ts) { m_Position += m_Velocity * ts; };
	void setStartPosition(glm::vec2 position) { m_Position = position; };

	glm::vec2 getVelocity() { return m_Velocity; };
	glm::vec4 getStartColor() { return m_StartColor; };
	glm::vec4 getEndColor() { return m_EndColor; };

	float getRotation() { return m_Rotation; };
	void setRotation(float ts) { m_Rotation += 0.01f * ts; };

	float getStartSize() { return m_StartSize; };
	float getEndSize() { return m_EndSize; };
	float getLifeTime() { return m_LifeTime; };

	float getLifeRemaining() { return m_LifeRemaining; };
	void setLifeRemaining(float ts) { m_LifeRemaining =- ts; };

	bool getStatus() { return m_Active; };
	void setStatus(bool status) { m_Active = status; };

	//std::unique_ptr<Shader> getShader() { return m_ParticleShader; };

	GLuint m_QuadVA = 0;

private:
	/* Main particle variables */
	glm::vec2 m_Position;
	glm::vec2 m_Velocity;
	glm::vec4 m_StartColor, m_EndColor;
	float m_Rotation = 0.0f;
	float m_StartSize, m_EndSize;

	float m_LifeTime = 1.0f;
	float m_LifeRemaining = 0.0f;

	bool m_Active = false;

	/* End of main particle variables */
};

