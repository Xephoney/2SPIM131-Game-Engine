#pragma once
#define _DEBUG

#include "PxPhysicsAPI.h"


namespace Engine
{
	using namespace physx;
	class PhysicsWorld
	{
	public:
		PhysicsWorld();
		void Initialize();

	private:
		//Functions

	private:
		// Variables
		PxDefaultAllocator callbackAllocator;
		PxDefaultErrorCallback errorCallback;
		PxTolerancesScale toleranceScale;
		PxFoundation* foundation			{ nullptr };
		PxPhysics* physics					{ nullptr };
		PxDefaultCpuDispatcher* dispatcher	{ nullptr };
		PxScene* scene						{ nullptr };
		PxMaterial* material				{ nullptr };
		PxPvd* pvd							{ nullptr };
		

	};


}
