#include <engpch.h>

#include "PhysicsWorld.h"
#include "Engine/Core.h"
#include "Engine/Log.h"

namespace Engine
{
	PhysicsWorld::PhysicsWorld()
	{
		Initialize();
	}
	
	void PhysicsWorld::Initialize()
	{
		btDefaultCollisionConfiguration* collConfig = new btDefaultCollisionConfiguration();
		btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collConfig);
		btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
		btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();

		btDiscreteDynamicsWorld* dynamics_world = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collConfig);

		dynamics_world->setGravity(btVector3{ 0.0,-9.81, 0.0 });

		btAlignedObjectArray<btCollisionShape*> collisionShapes;

		btCollisionShape* sphereShape = new btSphereShape(1.0);
		collisionShapes.push_back(sphereShape);

		btTransform sphereTransform;
		sphereTransform.setIdentity();
		sphereTransform.setOrigin(btVector3(0.0, 20.0, 0.0));
		btScalar mass = 2.0;

		bool isDynamic = (mass != 0.f);
		if (isDynamic)
			sphereShape->calculateLocalInertia(mass, btVector3(0.0, 0.0, 0.0));

		btDefaultMotionState* motionState = new btDefaultMotionState(sphereTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, sphereShape, btVector3(0.00, 0.00, 0.00));
		btRigidBody* body = new btRigidBody(rbInfo);
		dynamics_world->addRigidBody(body);

	}

	btRigidBody* PhysicsWorld::AddRigidbody()
	{
		return nullptr;
	}

	void PhysicsWorld::Tick(const double& timeStep)
	{
		dynamics_world->stepSimulation(1.0 / 120.0);

	}
}
