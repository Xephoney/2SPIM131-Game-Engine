#include <engpch.h>

#include "PhysicsWorld.h"
#include "Engine/Log.h"
#include "glm/glm.hpp"

namespace Engine
{

	PhysicsWorld::PhysicsWorld()
	{
		if (_instance)
		{
			delete this;
			return;
		}
		
		_instance = this;
		Initialize();
	}

	PhysicsWorld::~PhysicsWorld()
	{
		delete overlappingPairCache;
		delete collConfig;
		delete dispatcher;
		delete solver;
		delete dynamics_world;
	}

	void PhysicsWorld::Initialize()
	{
		overlappingPairCache = new btDbvtBroadphase();

		collConfig = new btDefaultCollisionConfiguration();
		dispatcher = new btCollisionDispatcher(collConfig);

		solver = new btSequentialImpulseConstraintSolver();

		dynamics_world = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collConfig);

		dynamics_world->setGravity(btVector3{ 0.0,-10, 0.0 });




	}

	btRigidBody* PhysicsWorld::NewRigidBody(float mass, const glm::vec3& position, const glm::vec3& rotation, CollisionShape collision_shape )
	{
		btTransform _transform = btTransform{
			btQuaternion(rotation.y, rotation.x, rotation.z),
			btVector3(position.x, position.y, position.z) };
		
		
		
		auto motionState = new btDefaultMotionState(_transform);
		btCollisionShape* col_shape {nullptr}; 
		switch (collision_shape)
		{
			case CollisionShape::Box:			col_shape = new btBoxShape({0.5,0.5 ,0.5 });
			case CollisionShape::Cylinder:		col_shape = new btCylinderShape({ 0.5,0.5,0.5 });
			case CollisionShape::Sphere:		col_shape = new btSphereShape(0.5);
			default: ;
		}
		btVector3 myStartingInertia(0, 0, 0);
		if(mass > 0.0)
			col_shape->calculateLocalInertia(mass, myStartingInertia);

		btRigidBody::btRigidBodyConstructionInfo myRigidBodyConstructionInfo(mass,
			motionState, col_shape,
			myStartingInertia);
		myRigidBodyConstructionInfo.m_restitution = 0.35;
		myRigidBodyConstructionInfo.m_friction = 0.0;
		
		btRigidBody* body = new btRigidBody(myRigidBodyConstructionInfo);
		dynamics_world->addRigidBody(body);
		return body;
	}

	void PhysicsWorld::Tick(const double& timeStep) const
	{
		dynamics_world->stepSimulation(timeStep*0.1,2);
	}
}
