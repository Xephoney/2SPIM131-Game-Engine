#pragma once

#include <Jolt/Jolt.h>

#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

#include "glm/trigonometric.hpp"
#include "glm/vec3.hpp"
#include "Jolt/Physics/Collision/ShapeCast.h"



class MyContactListener : public JPH::ContactListener
{

public:
	std::function<void(std::string clip)> callback;
	// See: ContactListener
	virtual JPH::ValidateResult	OnContactValidate(const JPH::Body& inBody1, const JPH::Body& inBody2, JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult& inCollisionResult) override
	{
		// Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
		return JPH::ValidateResult::AcceptContact;
	}

	virtual void			OnContactAdded(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings) override
	{

		if (inBody1.GetLinearVelocity().LengthSq() > 200 || inBody2.GetLinearVelocity().LengthSq() > 200)
			callback("Hit");
		//Called whenever a new contact point is detected.
		//Note that this callback is called when all bodies are locked, so don't use any locking functions!
		//Body 1 and 2 will be sorted such that body 1 ID < body 2 ID, so body 1 may not be dynamic.
		//Note that only active bodies will report contacts, as soon as a body goes to sleep the contacts between that body and all other bodies will receive an OnContactRemoved callback, if this is the case then Body::IsActive() will return false during the callback.
		//When contacts are added, the constraint solver has not run yet, so the collision impulse is unknown at that point.

		//The velocities of inBody1 and inBody2 are the velocities before the
		//		contact has been resolved, so you can use this to estimate the collision impulse
		//		to e.g. determine the volume of the impact sound to play.
	}

	virtual void			OnContactPersisted(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings) override
	{
		//ENGINE_LOG_INFO("A contact was persisted");
	}

};
class MyBodyActivationListener;
class BPLayerInterfaceImpl;

namespace Engine
{
	
	class PhysicsWorld
	{
	public:
		PhysicsWorld();

		~PhysicsWorld() = default;
		auto Initialize(bool withFloor = false) -> void;
		auto Tick(const double& timeStep) const -> void;
		auto GetInterface() const -> JPH::BodyInterface& { return physics_system->GetBodyInterface(); }
		auto CreateBoxBody(bool dynamic, const glm::vec3& _position, const glm::vec3& _eulRotation, const glm::vec3& halfExtent) const -> JPH::BodyID;
		auto CreateSphereBody(bool dynamic, const glm::vec3& _position, const glm::vec3& _eulRotation, const float& radius) const -> JPH::BodyID;
		auto SetGravity(const glm::vec3& dir) const -> void { physics_system->SetGravity({ dir.x, dir.y, dir.z }); }

		auto GetBodyPosition(const JPH::BodyID id, glm::vec3& in_position) const -> void
		{
			if (!system)
				return;
			const JPH::RVec3 newpos = physics_system->GetBodyInterface().GetCenterOfMassPosition(JPH::BodyID(id));
			in_position.x = newpos.GetX();
			in_position.y = newpos.GetY();
			in_position.z = newpos.GetZ();
		}
		auto GetBodyRotation(const JPH::BodyID id, glm::vec3& in_rotation) const  -> void
		{
			if (!system)
				return;
			const JPH::RVec3 newrot = physics_system->GetBodyInterface().GetRotation(id).GetEulerAngles();
			in_rotation.x = newrot.GetX();
			in_rotation.y = newrot.GetY();
			in_rotation.z = newrot.GetZ();
		}

		void SetBodyPosition(const JPH::BodyID id, const glm::vec3& rotation) const
		{
			if (!system)
				return;

			physics_system->GetBodyInterface().SetPosition(id,JPH::RVec3{ rotation.x, rotation.y, rotation.z }, JPH::EActivation::Activate);
		}
		void SetBodyRotation(const JPH::BodyID id, const glm::vec3& rotation) const
		{
			if (!system)
				return;

			physics_system->GetBodyInterface().SetRotation(id, JPH::QuatArg::sEulerAngles( {rotation.x, rotation.y, rotation.z }), JPH::EActivation::Activate);
		}

		template<typename Fnc>
		auto SetCollisionResponseFunction(Fnc&& lamda) const -> void;
		auto SphereOverlapCheck(const glm::vec3& inpos, const float& radius) -> void
		{
			//JPH::RShapeCast cast(JPH::SphereShape,JPH::Vec3(radius));
		}

		int EntityCount{ 0 };
		
	private :
		JPH::PhysicsSystem* physics_system { nullptr };
		JPH::TempAllocatorImpl* temp_alloc { nullptr };
		JPH::JobSystemThreadPool* job_system { nullptr};
		BPLayerInterfaceImpl* broad_phase_layer_interface{ nullptr };
		MyBodyActivationListener* body_activation_listener{ nullptr };
		MyContactListener* ContactListener{ nullptr };

		friend struct RigidBody;
		friend class Scene;

	};
	template <typename Fnc>
	auto PhysicsWorld::SetCollisionResponseFunction(Fnc&& lamda) const -> void
	{
		if(ContactListener)
			ContactListener->callback = lamda;
	}

	
}
