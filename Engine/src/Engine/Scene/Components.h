#pragma once
#include "glm/glm.hpp"
#include <string>

#include "MeshManager.h"
#include "Engine/Renderer/VertexArray.h"
#include "Engine/Audio/sound.h"



namespace Engine
{

	struct Transform
	{
		glm::mat4 transform{ 1.f };
		Transform() = default;
		Transform(const Transform&) = default;
		Transform(const glm::mat4& _transform)
			: transform(_transform) { }

		operator glm::mat4& () { return transform; }
		operator const glm::mat4& () const { return transform; }
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

	struct MeshRenderer
	{
		//Mesh m_Mesh;
		MeshRenderer()
		{
			vertexArray.reset(VertexArray::Create());
			float vertices[7 * 4]
			{
				-0.5f, -0.5f, 0.0f, 0.0f, 0.2f, 0.8f, 1.f,
				 0.5f, -0.5f, 0.0f, 0.9f, 0.0f, 0.3f, 1.f,
				 0.5f,  0.5f, 0.0f, 0.1f, 0.8f, 0.1f, 1.f,
				-0.5f,  0.5f, 0.0f, 0.0f, 0.2f, 0.8f, 1.f
			};

			// Vertex Buffer Stuff 
			std::shared_ptr<VertexBuffer> vertexBuffer;
			vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
			vertexBuffer->SetLayout({
				{ShaderDataType::Float3, "a_Position"},
				{ShaderDataType::Float4, "a_Color"}
			});
			vertexArray->AddVertexBuffer(vertexBuffer);

			
			// Index Buffer Stuff
			unsigned int indices[3 * 2]
			{ 0, 1, 2, 2, 3, 0 };

			std::shared_ptr<IndexBuffer> indexBuffer;
			indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));
			vertexArray->SetIndexBuffer(indexBuffer);


		};
		MeshRenderer(const MeshRenderer&) = default;
		//MeshRenderer(const Mesh& mesh) : m_Mesh(mesh) {};
		MeshRenderer(const std::string& filePath)
		{

		};

		std::shared_ptr<VertexArray> vertexArray;
		operator std::shared_ptr<VertexArray>& () { return vertexArray; }
		operator const std::shared_ptr<VertexArray>& () const { return vertexArray; }
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

	struct Rigidbody
	{
		
		int physxIndex;
		Rigidbody() = default;
		Rigidbody(const Rigidbody&) = default;
	};
}
