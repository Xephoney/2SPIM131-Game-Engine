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


class MyContactListener;
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
		auto Tick(const double& timeStep) -> void;
		auto GetInterface() -> JPH::BodyInterface& { return physics_system->GetBodyInterface(); }
		auto CreateBoxBody(bool dynamic, const glm::vec3& _position, const glm::vec3& _eulRotation, const glm::vec3& halfExtent) const -> JPH::BodyID;
		auto CreateSphereBody(bool dynamic, const glm::vec3& _position, const glm::vec3& _eulRotation, const float& radius) const -> JPH::BodyID;
		auto SetGravity(const glm::vec3& dir) -> void { physics_system->SetGravity({ dir.x, dir.y, dir.z }); }
		auto GetBodyPosition(const JPH::BodyID id, glm::vec3& in_position) const -> void
		{
			if (!system)
				return;

			JPH::RVec3 newpos = physics_system->GetBodyInterface().GetCenterOfMassPosition(JPH::BodyID(id));

			in_position.x = newpos.GetX();
			in_position.y = newpos.GetY();
			in_position.z = newpos.GetZ();
		}
		auto GetBodyRotation(const JPH::BodyID id, glm::vec3& in_rotation) const  -> void
		{
			if (!system)
				return;

			JPH::RVec3 newrot = physics_system->GetBodyInterface().GetRotation(id).GetEulerAngles();
			in_rotation.x = newrot.GetX();
			in_rotation.y = newrot.GetY();
			in_rotation.z = newrot.GetZ();
		}
		auto SphereOverlapCheck(const glm::vec3& inpos, const float& radius) -> void
		{
			//JPH::RShapeCast cast(JPH::SphereShape,JPH::Vec3(radius));
		}
		
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
}
