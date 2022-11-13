#include <engpch.h>

#include "Renderer.h"


#include "Renderer.h"

#include "Engine/Scene/Mesh.h"

namespace Engine
{

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::NewFrame(const Camera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProj();
	}

	void Renderer::RenderFrame()
	{

	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
	void Renderer::Submit(Shader& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader.Bind();
		shader.SetUniformMatrix4fv("u_model", transform);
		shader.SetUniformMatrix4fv("u_view_projection", m_SceneData->ViewProjectionMatrix);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
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
