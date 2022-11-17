#pragma once

#include "Camera.h"
#include "RenderCommand.h"
#include "Shader.h"

namespace Engine
{
	class Renderer
	{
	public:
		static void NewFrame(const Camera& camera);
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void RenderFrame();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);
		static void Submit(Shader& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.f));
		// static void SubmitInstanced(struct StaticMesh& staticMesh, const glm::mat4& transform = glm::mat4(1.f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); };

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
		// static std::vector<std::pair<uint32_t, glm::mat4>> instanceQueue;
	};
}
