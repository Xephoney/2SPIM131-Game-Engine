#include "engpch.h"
#include "Particle.h"
namespace Engine
{
	Particle::Particle()
	{
		mesh = MeshManager::instance().GetMesh("../Engine/Assets/3D/Primitives/split_cube.gltf");
	}

	void Particle::init()
	{
		//mesh = MeshManager::instance().GetMesh("../Engine/Assets/3D/Primitives/plane.gltf");
		mesh = MeshManager::instance().GetMesh("../Engine/Assets/3D/Primitives/split_cube.gltf");

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
		m_Velocity.z += particleProps.VelocityVariation.z * (Random::Float() - 0.5f);

		// Color
		m_StartColor = particleProps.ColorBegin;
		m_EndColor = particleProps.ColorEnd;

		m_LifeTime = particleProps.LifeTime;
		m_LifeRemaining = particleProps.LifeTime;
		m_StartSize = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Float() - 0.5f);
		m_EndSize = particleProps.SizeEnd;
	}
}
