#include <engpch.h>

#include "Renderer.h"


#include "Renderer.h"

#include "Engine/Scene/Mesh.h"

namespace Engine
{

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::NewFrame(std::shared_ptr<Camera> camera)
	{
		m_SceneData->ViewProjectionMatrix = camera->GetViewProj();
		m_SceneData->render_camera = camera;
		RenderCommand::Init();
	}

	void Renderer::NewFrame(const glm::mat4& projection, const glm::mat4& transform)
	{
		m_SceneData->ViewProjectionMatrix = projection * glm::inverse(transform);
		m_SceneData->render_camera = nullptr;
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::RenderFrame()
	{

	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
	void Renderer::Submit(Shader& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform, int ID)
	{
		shader.Bind();
		shader.SetMat4("u_model", transform);
		shader.SetMat4("u_view_projection", m_SceneData->ViewProjectionMatrix);
		shader.SetInt("u_ID", ID);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
		shader.Unbind();
		vertexArray->Unbind();
	}

	void Renderer::SubmitLines(Shader& shader, const std::shared_ptr<VertexArray>& vertexArray,
		const glm::mat4& transform, int ID)
	{
		shader.Bind();
		shader.SetMat4("u_model", transform);
		shader.SetMat4("u_view_projection", m_SceneData->ViewProjectionMatrix);
		shader.SetInt("u_ID", ID);

		vertexArray->Bind();
		RenderCommand::DrawLinesIndexed(vertexArray);
		shader.Unbind();
		vertexArray->Unbind();
	}

	// void Renderer::SubmitInstanced(StaticMesh& staticMesh, const glm::mat4& transform)
	// {
	// 	for(int i = 0; i < staticMesh.meshes.size(); i++)
	// 	{
	// 		instanceQueue.push_back({staticMesh.meshes[i], transform});
	// 	}
	// }
}
