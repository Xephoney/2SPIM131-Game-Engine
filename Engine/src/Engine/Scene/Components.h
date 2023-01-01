#pragma once
#include "glm/glm.hpp"
#include <string>

#include "MeshManager.h"
#include "Engine/Application.h"
#include "Engine/Application.h"
#include "Engine/Renderer/VertexArray.h"
#include "Engine/Audio/sound.h"
#include "glm/gtx/transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "Jolt/Jolt.h"
namespace Engine
{

	struct Transform
	{
		glm::mat4 transform			{ 1.f  };
		glm::vec3 position			{ 0.f };
		glm::vec3 euler_rotation	{ 0.f };
		glm::vec3 scale				{ 1.f };
		Transform() = default;
		Transform(const Transform&) = default;
		Transform(const glm::mat4& _transform)
			: transform(_transform)
		{
			CalculateTransform();
		}

		operator glm::mat4& ()
		{
			CalculateTransform();
			return transform;
		}
		operator const glm::mat4& () const
		{
			return transform;
		}
		inline void CalculateTransform()
		{
			glm::mat4 translation = glm::translate(glm::mat4{ 1.f }, position);
			glm::mat4 rotation = glm::toMat4(glm::quat(euler_rotation));
			//glm::mat4 rotation = glm::rotate(glm::mat4(1.f), glm::radians(euler_rotation.x), {1,0,0})
				//				* glm::rotate(glm::mat4(1.f), glm::radians(euler_rotation.y), { 0,1,0 })
					//			* glm::rotate(glm::mat4(1.f), glm::radians(euler_rotation.z), { 0,0,1 });

			transform = translation * rotation * glm::scale(glm::mat4{ 1.f }, scale);
		}
		void SetRotation(const glm::vec3& rot)
		{
			euler_rotation = rot;
		}

		void Rotate(const glm::vec3& deltaRotation)
		{
			euler_rotation += deltaRotation;
		}
	};

	struct Tag
	{
		std::string tag = "";
		Tag() = default;
		Tag(const std::string& name)
		{
			name.empty() ? tag = "empty" : tag = name;
		}
		operator std::string& () { return tag; }
		operator const std::string& () const { return tag; }
	};

	
	struct StaticMeshRenderer
	{
		StaticMesh mesh;
		StaticMeshRenderer() = default;
		StaticMeshRenderer(const StaticMeshRenderer&) = default;
		// StaticMeshRenderer(StaticMeshRenderer&& SMR) noexcept :
		// 	mesh(std::move(SMR.mesh)) { }
		StaticMeshRenderer(const std::string& filepath)
		{
			mesh = MeshManager::instance().GetMesh(filepath);
		}
		operator StaticMesh& () { return mesh; }
		operator const StaticMesh& () const { return mesh; }

		std::vector<Mesh> GetMeshes()
		{
			std::vector<Mesh> meshes;
			for(auto& s_mesh : mesh.meshes)
			{
				auto& mesh = MeshManager::instance().GetMeshFromID(s_mesh);
				
				meshes.emplace_back(mesh);
			}
		}
	};

	struct DebugBox
	{
		std::shared_ptr<VertexArray> va;
		DebugBox() = default;
		DebugBox(glm::vec4 color) : va(VertexArray::Create())
		{
			std::vector<Vertex> verts;
			std::vector<unsigned int> indices;
			float size = 0.5001;
			verts.emplace_back(Vertex({ size,-size,size }, { 0,0,0 }, color, { 0,0 }));
			verts.emplace_back(Vertex({ -size,-size,size }, { 0,0,0 }, color, { 0,0 }));
			verts.emplace_back(Vertex({ -size, size,size }, { 0,0,0 }, color, { 0,0 }));
			verts.emplace_back(Vertex({ size, size,size }, { 0,0,0 }, color, { 0,0 }));
			verts.emplace_back(Vertex({ size,-size,-size }, { 0,0,0 }, color, { 0,0 }));
			verts.emplace_back(Vertex({ -size,-size,-size }, { 0,0,0 }, color, { 0,0 }));
			verts.emplace_back(Vertex({ -size, size,-size }, { 0,0,0 }, color, { 0,0 }));
			verts.emplace_back(Vertex({ size, size,-size }, { 0,0,0 }, color, { 0,0 }));

			indices.push_back(0);
			indices.push_back(1);

			indices.push_back(0);
			indices.push_back(3);

			indices.push_back(0);
			indices.push_back(4);

			indices.push_back(1);
			indices.push_back(2);

			indices.push_back(1);
			indices.push_back(5);

			indices.push_back(5);
			indices.push_back(6);

			indices.push_back(5);
			indices.push_back(4);

			indices.push_back(4);
			indices.push_back(7);

			indices.push_back(7);
			indices.push_back(3);

			indices.push_back(3);
			indices.push_back(2);

			indices.push_back(2);
			indices.push_back(6);

			indices.push_back(6);
			indices.push_back(7);
			std::shared_ptr<VertexBuffer> vb;
			vb.reset(VertexBuffer::Create(verts));
			vb->SetLayout({
				{ShaderDataType::Float3, "a_Position"},
				{ShaderDataType::Float3, "a_Normal"},
				{ShaderDataType::Float4, "a_Color"},
				{ShaderDataType::Float2, "a_UV"}
				});
			va->AddVertexBuffer(vb);

			std::shared_ptr<IndexBuffer> ib;
			ib.reset(IndexBuffer::Create(indices.data(), indices.size()));
			va->SetIndexBuffer(ib);
		}
		Material mat;
		DebugBox(const DebugBox&) = default;
	};

	struct AudioListener
	{
		AudioListener(const AudioListener&) = default;
		AudioListener() = default;
		AudioListener(int i)
		{
			ENGINE_LOG_INFO("Created AudioListener component");
		}
		
		void playSound(std::string name) 
		{

		}
	};

	struct AudioSource
	{
		sound* mSound = nullptr;
		std::string mName = "DEFAULT NAME";
		AudioSource()
		{
			ENGINE_LOG_WARNING("AUDIOSOURCE COMPONENT ADDED, BUT NO SOUND OR MANAGER WAS PASSED IN");
		}
		AudioSource(sound* inSound) 
		{
			mSound = inSound;
			mName = mSound->getName();
		}
		AudioSource(std::string name)
		{
			mName = name;
			mSound = new sound(name);
		}
		
		void addSound(const char* file, std::string name)
		{
			mSound = new sound(file, name);
			mName = name;
		}


		void playSound()
		{
			mSound->playSound(mName);
		}
	};

	struct RigidBody
	{
		JPH::BodyID data {0};
		bool dynamic { true };
		RigidBody() = default;
		RigidBody(JPH::BodyID id) : data(id) { }
		RigidBody(const RigidBody&) = default;
		
	};

	/*struct Rigidbody_bt
	{
		Rigidbody()
		{
			mass = 1;
			velocity = 0;
		}
		Rigidbody(btRigidBody* rb)
			: internal_rb(rb)
		{
			mass = (float)rb->getMass();
			if (mass > 0.0)
				dynamic = true;

		}
		Rigidbody(const Rigidbody&) = default;

		bool dynamic{ false };

		float mass {1.0};
		float velocity {0.0};
		
		void AddForce(const glm::vec3& force)
		{
			internal_rb->applyCentralForce(btVector3(force.x, force.y, force.z));
		}
		
		void SetMass(const float& newMass)
		{
			mass = newMass;
			btVector3 inertia(0,0,0);
			if (mass > 0.f)
			{
				dynamic = true;
				internal_rb->setMassProps(mass, inertia);
			}
			else
			{
				
			}
			
				
		}
		btRigidBody* internal_rb{ nullptr };
	};
	*/

	struct CameraComponent
	{
		glm::mat4 Projection;
		std::shared_ptr<Camera> camera;
		bool Main = false;
		bool FixedAspectRatio = false;
		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const glm::mat4& proj) : Projection(proj)
		{
			
		}
	};
}
