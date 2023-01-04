#include <engpch.h>

#include "Renderer.h"


#include "Renderer.h"

#include "Engine/Scene/Mesh.h"
#include "glm/gtc/type_ptr.hpp"

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
	void Renderer::Submit(Shader& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform, int ID, const glm::vec4& color)
	{
		shader.Bind();
		shader.SetMat4("u_model", transform);
		shader.SetMat4("u_view_projection", m_SceneData->ViewProjectionMatrix);
		shader.SetInt("u_ID", ID);
		shader.SetFloat4("u_Color", color);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
		shader.Unbind();
		vertexArray->Unbind();
	}

	void Renderer::SubmitCubeFrame(glm::vec4 Color, const glm::mat4& transform, int ID)
	{
		//std::shared_ptr<Shader> shader = m_SceneData->shader_library.Get("PlainShader");
		ENGINE_CORE_ASSERT(false, "FUNCTION NOT IMPLEMENTED YET")
	}


	void Renderer::SubmitParticle(const std::shared_ptr<Shader>& shader, const glm::vec4& color, const std::shared_ptr<VertexArray>& va, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetFloat4("u_Color", color);
		
		shader->SetMat4("u_model", transform);
		shader->SetMat4("u_view_projection", m_SceneData->ViewProjectionMatrix);
		va->Bind();
		RenderCommand::DrawIndexed(va);
		shader->Unbind();
		va->Unbind();
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

	void Renderer::SubmitDepthpass(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& va,
		const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetMat4("u_model", transform);
		va->Bind();
		RenderCommand::DrawIndexed(va);
		va->Unbind();
		shader->Unbind();
		
	}

	void Renderer::SubmitDirectionalLight(Shader& shader, 
	                                      const glm::vec3& direction,
	                                      int specularExponent, 
	                                      float specularStrength , 
	                                      float ambientStrength ,
	                                      const glm::vec3& lightColor)
	{
		shader.Bind();
		shader.SetFloat3("lightDirection", direction);
		shader.SetFloat3("cameraPosition", m_SceneData->render_camera->GetPosition());
		shader.SetInt("specularExponent", specularExponent);
		shader.SetFloat("specularStrength", specularStrength);
		shader.SetFloat("ambientStrength", ambientStrength);
		shader.SetFloat3("lightColor", lightColor);
	}

	void Renderer::SubmitDirectionalLightShadow(Shader& shader, const glm::vec3& direction,
		const glm::mat4& lightSpaceMatrix, int specularExponent, float specularStrength, float ambientStrength,
		const glm::vec3& lightColor)
	{
		shader.Bind();
		shader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);
		SubmitDirectionalLight(shader, direction, specularExponent, specularStrength, ambientStrength, lightColor);
	}


	// void Renderer::SubmitInstanced(StaticMesh& staticMesh, const glm::mat4& transform)
	// {
	// 	for(int i = 0; i < staticMesh.meshes.size(); i++)
	// 	{
	// 		instanceQueue.push_back({staticMesh.meshes[i], transform});
	// 	}
	// }
}
