#pragma once

#include <vector>


namespace Engine
{
	class ParticleSystem
	{
	public:
		ParticleSystem();

		void Update(const double& dt); 
		void Render(); 
		void Emit(class Emitter& emitter);
		void init();

	private:
		std::vector<class Emitter> m_EmitterPool; // pool of all particles
		int numOfEmitters = 10;
};}

