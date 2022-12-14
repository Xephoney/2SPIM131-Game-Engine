// ReSharper disable CppClangTidyModernizeUseNodiscard
#pragma once
#include "glm/glm.hpp"
#include <string>

#include "MeshManager.h"
#include "Engine/Application.h"
#include "Engine/Renderer/VertexArray.h"
#include "Engine/Audio/sound.h"
#include "Engine/Renderer/Framebuffer.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Engine/Renderer/DepthOnlyFrameBuffer.h"

namespace Engine
{

	struct Transform
	{
	private:
		glm::vec3 int_pos{ 0.f };
		glm::vec3 int_rot{ 0.f };
		glm::vec3 int_scl{ 1.f };

		float epsilon{ 0.0005f };
		bool check() const 
		{
			return (glm::length2(int_pos - position) > epsilon ||
					glm::length2(int_rot - euler_rotation) > epsilon ||
					glm::length2(int_scl - scale) > epsilon);
		}
	public:
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
			if (check())
			{
				int_pos = position;
				int_rot = euler_rotation;
				int_scl = scale;
				CalculateTransform();
			}
			return transform;
		}

		operator const glm::mat4& () const
		{
			return transform;
		}
		inline void CalculateTransform()
		{
			const glm::mat4 translation = glm::translate(glm::mat4{ 1.f }, position);
			const glm::mat4 rotation = glm::toMat4(glm::quat(euler_rotation));
			
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

		glm::vec3 Forward() const
		{
			return glm::normalize(glm::vec3(transform[2]));
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
		glm::vec4 color{ 1,1,1,1 };
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
			return meshes;
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
			mat = { "../Engine/Assets/Shaders/PlainUnlitShader.glsl" };
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
	private:
		sound* mSound = nullptr;
		
		FMOD_VECTOR mPos;
		float minDist = 5.f;
		float maxDist = 10.f;
	public:
		std::string mName = "DEFAULT NAME";
		// constructors
		AudioSource()
		{
			ENGINE_LOG_WARNING("AUDIOSOURCE COMPONENT ADDED, BUT NO SOUND, SOUNDMANAGER OR POSITION WAS PASSED IN");
		}
		AudioSource(sound* inSound) 
		{
			mSound = inSound;
			mName = setIdentifyer(mSound->getName());
		}
		AudioSource(std::string name)
		{
			mName = setIdentifyer(name);
			mSound = new sound(setIdentifyer(name));
			SoundManager::getSoundManager().addToSoundlist(mSound);
		}
		AudioSource(sound* inSound, glm::vec3 pos)
		{
			mSound = inSound;
			mName=setIdentifyer(mSound->getName());
			setPos(pos);
			mSound->setMaxDist(maxDist);
			mSound->setMinDist(minDist);
			mSound->setPos(pos);
			SoundManager::getSoundManager().addToSoundlist(mSound);
		}
		AudioSource(std::string name, glm::vec3 pos)
		{
			mName=setIdentifyer(name);
			setPos(pos);
			mSound = new sound(setIdentifyer(name));
			mSound->setMaxDist(maxDist);
			mSound->setMinDist(minDist);
			mSound->setPos(pos);
			SoundManager::getSoundManager().addToSoundlist(mSound);
			
		}

		//Other functions
		void addSound(const char* file, std::string name)
		{
			mSound = new sound(file, setIdentifyer(name), false);
			mName = setIdentifyer(name);
		}
		void add3DSound(const char* file, std::string name, glm::vec3 pos)
		{
			mPos.x = pos.x;
			mPos.y = pos.y;
			mPos.z = pos.z;
			mSound = new sound(file, name, true);
			mSound->setPos(mPos);
			mName = setIdentifyer(name);
		}
		void swapSound(std::string name) {
			//Swap to an existing sound
			mName = setIdentifyer(name);
			bool success = mSound->swapSound(mName);
			if(!success)
				ENGINE_LOG_WARNING("could not swap sound to {0}",name);
			
		}
		void playSound()
		{
			mSound->playSound(mName);
		}

		void setPos(glm::vec3 in)
		{
			mPos.x = in.x;
			mPos.y = in.y;
			mPos.z = in.z;
		}

		FMOD_VECTOR getFMODpos() 
		{
			return mPos;
		}

		glm::vec3 getVec3Pos()
		{
			glm::vec3 temp;
			temp.x = mPos.x;
			temp.y = mPos.y;
			temp.z = mPos.z;
			return temp;
		}

		float getMinDist()
		{
			return minDist;
		}
		float getMaxDist()
		{
			return maxDist;
		}

		std::string setIdentifyer(std::string name) 
		{
			int num = SoundManager::getSoundManager().getExistingSoundsWithName(name);
			std::string newName = name + "ID:" + std::to_string(num);
			return newName;
		}
	};

	struct RigidBody
	{
		JPH::BodyID data {0};
		bool box = true;
		bool dynamic { true };
		bool show_collider{ true };
		RigidBody() = default;
		RigidBody(JPH::BodyID id) : data(id) {  }
		RigidBody(const RigidBody&) = default;
		
	};

	struct DirectionalLight
	{
		float intensity { 1.f };
		glm::vec3 lightColor { 1.f };
	
		int specularExponent = 128;
		float specularStrength = 0.9;
		float ambientStrength = 0.45;
		std::shared_ptr<Shader> lightShader;
		std::shared_ptr<Shader> dirLightShader;
		std::shared_ptr<Shader> shadowMapShader;
		std::shared_ptr<DepthOnlyFramebuffer> shadowFrameBuffer;
		glm::mat4 projectionMatrix{1.f};
		glm::mat4 viewMatrix{ 1.f };
		glm::mat4 lightSpaceMatrix{ 1.f };
		DirectionalLight()
		{
			lightShader = ShaderLibrary::instance().Get("PlainShader");
			dirLightShader = ShaderLibrary::instance().Get("DirectionalShadows");
			shadowMapShader = ShaderLibrary::instance().Get("DirectionalShadows");
			
			shadowFrameBuffer = DepthOnlyFramebuffer::Create(4096);
			projectionMatrix = glm::ortho(-30.0, 30.0, -30.0, 30.0, -10.0, 40.0);
		}

		DirectionalLight(const DirectionalLight&) = default;

		void UpdateViewProjectionMatrix(const glm::vec3& direction)
		{
			glm::vec3 pos = 20.f * direction;
			viewMatrix = glm::lookAt(pos, glm::vec3(0.f), {0,1,0});
			lightSpaceMatrix = projectionMatrix * viewMatrix;
			shadowMapShader->Bind();
			shadowMapShader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);
			lightShader->Bind();
			lightShader->SetMat4("u_biasedLight", lightSpaceMatrix);
		}
		glm::mat4 GetBiasedViewProjection() const {
			return glm::mat4(
				0.5, 0.0, 0.0, 0.0,
				0.0, 0.5, 0.0, 0.0,
				0.0, 0.0, 0.5, 0.0,
				0.5, 0.5, 0.5, 1.0
			) * lightSpaceMatrix;
		}
	};
	
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

	struct ParticleProperties // Used to set the properties for a given particle.
	{
		glm::vec3 Position;
		glm::vec3 Velocity, VelocityVariation;
		glm::vec4 ColorBegin{ 1,0,0,1 }, ColorEnd {0,1,0,1};
		float SizeBegin{ 1.f };
		float SizeEnd{ 0.1f };
		float SizeVariation {0.5f};
		float LifeTime = 1.0f;
	};

	struct EmitterProperties // Used to set the properties for a given emitter
	{
		glm::vec3 position;
		float lifeTime;
	};

	struct EmitterComponent
	{
		ParticleProperties particle_properties;
		EmitterComponent() = default;
		EmitterComponent(const EmitterComponent&) = default;

	};

	struct BombComponent
	{
		float maxRadius {30.f};
		float minRadius {10.f};

		float impulsePower {25000.f};

		std::shared_ptr<VertexArray> visualizer;
		
		BombComponent() = default;
		BombComponent(const BombComponent&) = default;

		inline void Explode() const
		{
			
		}
	};

}
