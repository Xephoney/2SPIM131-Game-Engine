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
#include "Emitter.h"

class ParticleSystem
{
	ParticleSystem(int numOfEmitters);

	void Update(float ts); 
	void Render(); 
	void Emit(std::vector<Emitter>& emitter);
	void init(Engine::Camera& camera);

private:
	std::vector<Emitter> m_EmitterPool; // pool of all particles

	GLuint m_QuadVA = 0;
	std::unique_ptr<Shader> m_ParticleShader; // pointer to the shader program
	GLint m_ParticleShaderViewProj, m_ParticleShaderTransform, m_ParticleShaderColor; // opengl variables

	int m_numOfEmitters = 10;
};

