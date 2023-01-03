#include <engpch.h>


#include "PhysicsWorld.h"

#include "engine/log.h"

using namespace JPH;

using namespace JPH::literals;

namespace Layers
{
	static constexpr uint8 NON_MOVING = 0;
	static constexpr uint8 MOVING = 1;
	static constexpr uint8 NUM_LAYERS = 2;
};

static bool MyObjectCanCollide(ObjectLayer inObject1, ObjectLayer inObject2)
{
	switch (inObject1)
	{
	case Layers::NON_MOVING:
		return inObject2 == Layers::MOVING; // Non moving only collides with moving
	case Layers::MOVING:
		return true; // Moving collides with everything
	default:
		JPH_ASSERT(false);
		return false;
	}
};

namespace BroadPhaseLayers
{
	static constexpr BroadPhaseLayer NON_MOVING(0);
	static constexpr BroadPhaseLayer MOVING(1);
	static constexpr uint NUM_LAYERS(2);
};

class BPLayerInterfaceImpl final : public BroadPhaseLayerInterface
{
public:
	BPLayerInterfaceImpl()
	{
		// Create a mapping table from object to broad phase layer
		mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
		mObjectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;
	}

	virtual uint					GetNumBroadPhaseLayers() const override
	{
		return BroadPhaseLayers::NUM_LAYERS;
	}

	virtual BroadPhaseLayer			GetBroadPhaseLayer(ObjectLayer inLayer) const override
	{
		JPH_ASSERT(inLayer < Layers::NUM_LAYERS);
		return mObjectToBroadPhase[inLayer];
	}

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
	virtual const char* GetBroadPhaseLayerName(BroadPhaseLayer inLayer) const override
	{
		switch ((BroadPhaseLayer::Type)inLayer)
		{
		case (BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING:	return "NON_MOVING";
		case (BroadPhaseLayer::Type)BroadPhaseLayers::MOVING:		return "MOVING";
		default:													JPH_ASSERT(false); return "INVALID";
		}
	}
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

private:
	BroadPhaseLayer					mObjectToBroadPhase[Layers::NUM_LAYERS];
};

class MyBodyActivationListener : public BodyActivationListener
{
public:
	virtual void		OnBodyActivated(const BodyID& inBodyID, uint64 inBodyUserData) override
	{
		ENGINE_LOG_INFO("PHYSICS :: A body got activated");
	}

	virtual void		OnBodyDeactivated(const BodyID& inBodyID, uint64 inBodyUserData) override
	{
		ENGINE_LOG_INFO("PHYSICS :: A body went to sleep");
	}
};


// Function that determines if two broadphase layers can collide
static bool MyBroadPhaseCanCollide(ObjectLayer inLayer1, BroadPhaseLayer inLayer2)
{
	switch (inLayer1)
	{
	case Layers::NON_MOVING:
		return inLayer2 == BroadPhaseLayers::MOVING;
	case Layers::MOVING:
		return true;
	default:
		JPH_ASSERT(false);
		return false;
	}
}
static void TraceImpl(const char* inFMT, ...)
{
	ENGINE_LOG_TRACE(inFMT);
}
// An example contact listener
class MyContactListener : public ContactListener
{
public:
	// See: ContactListener
	virtual ValidateResult	OnContactValidate(const Body& inBody1, const Body& inBody2, RVec3Arg inBaseOffset, const CollideShapeResult& inCollisionResult) override
	{
		// Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
		return ValidateResult::AcceptContact;
	}

	virtual void			OnContactAdded(const Body& inBody1, const Body& inBody2, const ContactManifold& inManifold, ContactSettings& ioSettings) override
	{
	
		//Called whenever a new contact point is detected.
		//Note that this callback is called when all bodies are locked, so don't use any locking functions!
		//Body 1 and 2 will be sorted such that body 1 ID < body 2 ID, so body 1 may not be dynamic.
		//Note that only active bodies will report contacts, as soon as a body goes to sleep the contacts between that body and all other bodies will receive an OnContactRemoved callback, if this is the case then Body::IsActive() will return false during the callback.
		//When contacts are added, the constraint solver has not run yet, so the collision impulse is unknown at that point.

		//The velocities of inBody1 and inBody2 are the velocities before the
		//		contact has been resolved, so you can use this to estimate the collision impulse
		//		to e.g. determine the volume of the impact sound to play.
	}

	virtual void			OnContactPersisted(const Body& inBody1, const Body& inBody2, const ContactManifold& inManifold, ContactSettings& ioSettings) override
	{
		//ENGINE_LOG_INFO("A contact was persisted");
	}

	virtual void			OnContactRemoved(const SubShapeIDPair& inSubShapePair) override
	{
		//ENGINE_LOG_INFO("A contact was removed");
	}
};

namespace Engine
{
	PhysicsWorld::PhysicsWorld() = default;
	

	void PhysicsWorld::Initialize(bool withFloor)
	{
		// Register allocation hook
		RegisterDefaultAllocator();
		Trace = TraceImpl;
		Factory::sInstance = new Factory();
		RegisterTypes();

		constexpr uint cMaxBodies = 65536;

		constexpr uint cNumMutexes = 0;

		constexpr uint cMaxBodyPairs = 65536;

		constexpr uint cMaxContactConstraints = 10240;

		broad_phase_layer_interface = new BPLayerInterfaceImpl;
		physics_system = new PhysicsSystem();
		physics_system->Init(cMaxBodies,
			cNumMutexes,
			cMaxBodyPairs,
			cMaxContactConstraints,
			*broad_phase_layer_interface,
			MyBroadPhaseCanCollide,
			MyObjectCanCollide
		);

		ContactListener = new MyContactListener();
		physics_system->SetContactListener(ContactListener);

		body_activation_listener = new MyBodyActivationListener();
		physics_system->SetBodyActivationListener(body_activation_listener);

		physics_system->SetGravity({ 0,-10.0,0 });
		// Used to interface with (Rigid)bodies
		BodyInterface& body_interface = physics_system->GetBodyInterface();
		if(withFloor)
		{
			BoxShapeSettings floor_shape_settings(Vec3(1000.0f, 1.0f, 1000.0f));

			// Create the shape
			ShapeSettings::ShapeResult floor_shape_result = floor_shape_settings.Create();
			ShapeRefC floor_shape = floor_shape_result.Get(); // We don't expect an error here, but you can check floor_shape_result for HasError() / GetError()

			BodyCreationSettings floor_settings(floor_shape, RVec3(0.0_r, -1.0_r, 0.0_r), Quat::sIdentity(), EMotionType::Static, Layers::NON_MOVING);
			body_interface.CreateAndAddBody(floor_settings, EActivation::DontActivate);
		}
		//physics_system.OptimizeBroadPhase();

		temp_alloc = new TempAllocatorImpl(100 * 1024 * 1024);
		job_system = new JobSystemThreadPool(2048, 8, thread::hardware_concurrency() - 1);
	}

	void PhysicsWorld::Tick(const double& timeStep)
	{
		physics_system->Update(timeStep, 1, 1, temp_alloc, job_system);
	}

	auto PhysicsWorld::CreateBoxBody(bool dynamic, const glm::vec3& _position,
		const glm::vec3& _eulRotation, const glm::vec3& halfExtent) const -> BodyID
	{
		BodyInterface& interface = physics_system->GetBodyInterface();
		EMotionType type;
		dynamic ? type = EMotionType::Dynamic : type = EMotionType::Static;

		const BodyCreationSettings settings(
			new BoxShape({ halfExtent.x,halfExtent.y,halfExtent.z }),
			{_position.x, _position.y, _position.z},
			Quat::sEulerAngles({ _eulRotation.x, _eulRotation.y, _eulRotation.z }),
			type,
			Layers::MOVING
			);
		Body* bdy = interface.CreateBody(settings);
		
		interface.AddBody(bdy->GetID(), EActivation::Activate);
		
		return bdy->GetID();
	}

	auto PhysicsWorld::CreateSphereBody(bool dynamic, const glm::vec3& _position, const glm::vec3& _eulRotation,
		const float& radius) const -> JPH::BodyID
	{
		BodyInterface& interface = physics_system->GetBodyInterface();
		EMotionType type;
		dynamic ? type = EMotionType::Dynamic : type = EMotionType::Static;

		const BodyCreationSettings settings(
			new SphereShape(radius),
			{ _position.x, _position.y, _position.z },
			Quat::sEulerAngles({ _eulRotation.x, _eulRotation.y, _eulRotation.z }),
			type,
			Layers::MOVING
		);
		return interface.CreateAndAddBody(settings, EActivation::Activate);
	}
}
