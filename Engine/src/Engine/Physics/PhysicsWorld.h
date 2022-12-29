#pragma once
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include <glm/vec3.hpp>
namespace Engine
{
	enum class CollisionShape
	{
		Box,
		Sphere,
		Cylinder
	};

	class PhysicsWorld
	{

	public:
		
		~PhysicsWorld();
		void Tick(const double& timeStep) const;

		btRigidBody* NewRigidBody(float mass, const glm::vec3& position, const glm::vec3& rotation, CollisionShape collision_shape);

	private:
		PhysicsWorld();
		PhysicsWorld(PhysicsWorld const&) = delete;
		void operator=(PhysicsWorld const&) = delete;
		//Functions
		void Initialize(); 
	private:
		// Variables
		btBroadphaseInterface*					overlappingPairCache {nullptr};
		btDefaultCollisionConfiguration*		collConfig{ nullptr };
		btCollisionDispatcher*					dispatcher {nullptr};
		btSequentialImpulseConstraintSolver*	solver {nullptr};
		btDiscreteDynamicsWorld*				dynamics_world {nullptr};

		friend class Scene;

		inline static PhysicsWorld* _instance {nullptr};
	};
}
