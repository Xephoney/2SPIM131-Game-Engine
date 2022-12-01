#pragma once
#include "Vertex.h"
#include "vector"
#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/Shader.h"

namespace Engine
{
	struct Material
	{
		
		glm::vec3 diffuse{};
		glm::vec3 ambient{};
		glm::vec3 specular{};
		glm::vec3 roughness{};
		glm::vec3 emissive{};
		glm::vec4 emissive_color{};

		float shininess = 0.f;
		bool twoSided = false;
		std::shared_ptr<Shader> shader;

		Material() : ambient(glm::vec3(1.f)), diffuse(glm::vec3(0.f)), specular(glm::vec3(0.f)), roughness(1.f),
		             emissive(0.f),
		             emissive_color(0.f)
		{
			// Create Shader
			std::string vertexShaderTemp = R"(
				#version 450 core
				layout(location = 0) in vec3 a_Position;
				layout(location = 1) in vec4 a_Color;
				layout(location = 2) in vec3 a_Normal;

				uniform mat4 u_view_projection;
				uniform mat4 u_model;

				out vec3 v_pos;
				out vec4 v_color;

				void main()
				{
					gl_Position = u_view_projection * u_model * vec4(a_Position,1);
					v_pos = a_Position;
					v_color = a_Color;
				}
			)";
			std::string fragmentShaderTemp = R"(
				#version 450 core
				layout(location = 0) out vec4 fragmentColor;
				layout(location = 1) out int index;

				in vec3 v_pos;
				in vec4 v_color;
			
				uniform int u_ID;


				void main()
				{
					fragmentColor = v_color;
					index = u_ID;
				}
			)";

			//shader = Shader::Create("plainShader", vertexShaderTemp, fragmentShaderTemp);
			shader = Shader::Create("../Engine/Assets/Shaders/PlainShader.glsl");
		}

		Material(const Material&) = default;
		Material(const std::string& ShaderFilePath) :
			ambient(glm::vec3(1.f)),
			diffuse(glm::vec3(0.f)),
			specular(glm::vec3(0.f)),
			roughness(1.f),
			emissive(0.f),
			emissive_color(0.f)
		{
			shader = Shader::Create(ShaderFilePath);
		}

		operator Shader& () { return *shader; }
		operator const Shader& () const { return *shader; }
	};

	struct StaticMesh
	{
		std::vector<uint32_t> meshes;
	};

	class Mesh
	{
	public:
		Mesh();
		Mesh(const std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
		~Mesh();
		std::shared_ptr<VertexArray> vertexArray;
		Material material;
	};

	inline Mesh::Mesh(const std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
	{
		vertexArray.reset(VertexArray::Create());
		std::shared_ptr<VertexBuffer> vb;
		vb.reset(VertexBuffer::Create(vertices, sizeof(Vertex) * vertices.size()));
		vb->SetLayout({
				{ShaderDataType::Float3, "a_Position"},
				{ShaderDataType::Float3, "a_Normal"},
				{ShaderDataType::Float4, "a_Color"},
				{ShaderDataType::Float2, "a_UV"}
			});
		vertexArray->AddVertexBuffer(vb);

		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices.data(), indices.size()));
		vertexArray->SetIndexBuffer(indexBuffer);
	}

	inline Mesh::~Mesh()
	{

	}
}
