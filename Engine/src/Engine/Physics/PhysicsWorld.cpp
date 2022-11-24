
#include <engpch.h>
#include "PhysicsWorld.h"

#include "Engine/Core.h"


namespace Engine
{
	PhysicsWorld::PhysicsWorld()
	{
		Initialize();
	}

	void PhysicsWorld::Initialize()
	{
		foundation = PxCreateFoundation(PX_PHYSICS_VERSION, callbackAllocator, errorCallback);
		ENGINE_CORE_ASSERT(foundation, "Physics Foundation was not created properly")
		pvd = PxCreatePvd(*foundation);
		PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
		pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
		toleranceScale.length = 100;
		toleranceScale.speed = 981;
		physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, toleranceScale, true, pvd);
		PxSceneDesc sceneDesc(physics->getTolerancesScale());
		sceneDesc.gravity = PxVec3{ 0.0f, -9.81f, 0.0f };
		dispatcher = PxDefaultCpuDispatcherCreate(4);
		sceneDesc.cpuDispatcher = dispatcher;
		sceneDesc.filterShader = PxDefaultSimulationFilterShader;
		scene = physics->createScene(sceneDesc);
	}
}
