#pragma once
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"

namespace Engine
{
	class PhysicsWorld
	{
	public:
		PhysicsWorld();
		void Initialize();
		void Tick(const double& timeStep) const;

		btRigidBody* AddRigidbody();

	private:
		//Functions

	private:
		// Variables
		btDiscreteDynamicsWorld* dynamics_world;


	};
}
