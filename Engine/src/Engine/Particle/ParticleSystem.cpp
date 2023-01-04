#include <engpch.h>
#include "ParticleSystem.h"
#include "Emitter.h"


namespace Engine
{
	ParticleSystem::ParticleSystem()
	{
		m_EmitterPool.resize(numOfEmitters);
	}

	void ParticleSystem::Update(const double& dt)
	{
		for (auto& emitter : m_EmitterPool)
		{
			if (emitter.getLifeRemaining() > 0 && emitter.Active)
			{
				emitter.UpdateParticles(dt);
			}
		}
	}

	void ParticleSystem::Render()
	{
		for (auto& emitter : m_EmitterPool)
		{
			if (emitter.getLifeRemaining() > 0 && emitter.Active)
			{
				emitter.EmitParticles();
			}
		}
	}

	/* used to initialize a particle and give it properties */
	void ParticleSystem::Emit(Emitter& emitter)
	{
		m_EmitterPool.push_back(emitter);
	}

	void ParticleSystem::init()
	{
		
	}
}
