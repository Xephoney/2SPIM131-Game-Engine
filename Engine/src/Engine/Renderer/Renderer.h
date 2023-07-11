#pragma once

#include "Camera.h"
#include "DebugShapes.h"
#include "RenderCommand.h"
#include "Shader.h"

namespace Engine
{
	class Renderer
	{
	public:
		static void NewFrame(std::shared_ptr<Camera> camera);
		static void NewFrame(const glm::mat4& projection, const glm::mat4& transform);
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void RenderFrame();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);
		static void Submit(Shader& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.f), int ID = -1, const glm::vec4& color = {1,1,1,1});
		static void SubmitCubeFrame(glm::vec4 Color, const glm::mat4& transform = glm::mat4(1.f), int ID = -1);
		static void SubmitParticle(const std::shared_ptr<Shader>& shader, const glm::vec4& color, const std::shared_ptr<VertexArray>& va, const glm::mat4& transform = glm::mat4(1.f));
		static void SubmitLines(Shader& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.f), int ID = -1);
		static void SubmitDepthpass(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& va, const glm::mat4& transform = glm::mat4(1.f));
		static void SubmitDirectionalLight(Shader& shader, const glm::vec3& direction, int specularExponent = 128,float specularStrength = 0.9f,float ambientStrength = 0.45f,const glm::vec3& lightColor = glm::vec3(1, 1, 1));
		static void SubmitDirectionalLightShadow(Shader& shader, const glm::vec3& direction, const glm::mat4& lightSpaceMatrix, int specularExponent = 128, float specularStrength = 0.9f, float ambientStrength = 0.45f, const glm::vec3& lightColor = glm::vec3(1, 1, 1));
		static void SubmitInstanced(struct StaticMesh& staticMesh, const glm::mat4& transform = glm::mat4(1.f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); };
		inline static std::shared_ptr<Camera> GetRenderCamera()  { return m_SceneData->render_camera; }
		//inline static ShaderLibrary& GetShaderLibrary() { return m_SceneData->shader_library; }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
			std::shared_ptr<Camera> render_camera;
			//ShaderLibrary shader_library;
			DebugShapes debug_stuff;
		};

		static SceneData* m_SceneData;

		// static std::vector<std::pair<uint32_t, glm::mat4>> instanceQueue;
	};
}
