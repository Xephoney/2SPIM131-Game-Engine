#include "engpch.h"

#include "Engine/engpch.h"

#include "Shader.h"

#include <fstream>

#include "Renderer.h"
#include "Engine/Log.h"
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"
#include "Platform/OpenGLShader.h"

namespace Engine
{

	std::shared_ptr<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::none: 
				ENGINE_CORE_ASSERT(false, "Cannot create shader with the \"none\" api selected");
				return nullptr;

			case RendererAPI::API::OpenGL: 
				return std::make_shared<OpenGLShader>(filepath);

		default: 
			return nullptr;
		}
	}

	std::shared_ptr<Shader> Shader::Create(const std::string& name, const std::string& vertex,
		const std::string& fragment)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::none:
			ENGINE_CORE_ASSERT(false, "Cannot create shader with the \"none\" api selected");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(name, vertex, fragment);

		default:
			return nullptr;
		}
	}

	void ShaderLibrary::Add(const std::string& name, const std::shared_ptr<Shader>& shader)
	{
		ENGINE_CORE_ASSERT(!Exists(name), "Shader already exists!")
		m_shaders[name] = shader;
	}

	bool ShaderLibrary::Add(const std::shared_ptr<Shader>& shader)
	{
		auto& name = shader->GetName();
		if (Exists(name))
		{
			return false;
		}
		Add(name, shader);
		return true;
	}

	std::shared_ptr<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		if (Add(shader))
			return shader;
		
		std::string name = shader->GetName();
		shader.reset();
		return m_shaders[name];
	
	}

	std::shared_ptr<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		if (Exists(name))
			return m_shaders[name];

		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	std::shared_ptr<Shader> ShaderLibrary::Get(const std::string& name)
	{
		ENGINE_CORE_ASSERT(Exists(name), "Shader not found!");
		return m_shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_shaders.find(name) != m_shaders.end();
	}

	ShaderLibrary::ShaderLibrary()
	{
		Load("../Engine/Assets/Shaders/PlainShader.glsl");
		Load("../Engine/Assets/Shaders/PlainUnlitShader.glsl");
		Load("../Engine/Assets/Shaders/DirectionalShadows.glsl");
	}
}


