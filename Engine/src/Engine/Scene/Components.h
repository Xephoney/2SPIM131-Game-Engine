#pragma once
#include "glm/glm.hpp"
#include <string>

#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/VertexArray.h"


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
		std::string tag;
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

	struct Material
	{
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float shininess = 0.f;
		Shader* shader;
		Material() 
		{
			// Create Shader
			std::string vertexShaderTemp = R"(

				#version 410 core
				layout(location = 0) in vec3 a_Position;
				layout(location = 1) in vec4 a_Color;
				layout(location = 2) in vec3 a_Normal;

				uniform mat4 view_projection;
				uniform mat4 model;

				out vec3 v_pos;
				out vec4 v_color;
				void main()
				{
					gl_Position = view_projection * model * vec4(a_Position,1);
					v_pos = a_Position;
					v_color = a_Color;
				}
			)";
			std::string fragmentShaderTemp = R"(

				#version 410 core
				layout(location = 0) out vec4 fragmentColor;
				in vec3 v_pos;
				in vec4 v_color;

				void main()
				{
					fragmentColor = v_color;
				}
			)";

			shader = new Shader{vertexShaderTemp, fragmentShaderTemp};
		}
		;
		Material(const Shader& shader);
		Material(const Material&) = default;

		operator Shader& () { return *shader; }
		operator const Shader& () const { return *shader; }

	};

	struct AudioListener
	{

	};
}
