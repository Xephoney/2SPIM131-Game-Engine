#pragma once
#include <string>
#include <glm/glm.hpp>
#include <memory>
#include <unordered_map>


namespace Engine
{
	class Shader 
	{
		
	public:
		virtual ~Shader() = default;

		static std::shared_ptr<Shader> Create(const std::string& filepath);
		static std::shared_ptr<Shader> Create(const std::string& name, const std::string& vertex, const std::string& fragment);

	    ///Use the current shader
	    virtual void Bind() const = 0;
	    virtual void Unbind() const = 0;

		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) = 0;
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
		virtual void SetMat3(const std::string& name, const glm::mat3& value) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;

	    ///Returns the program number for this shader
	    
		virtual const std::string& GetName() const = 0;

	private:
	    std::string m_filePath;
	};

	class ShaderLibrary
	{
	public:
		static ShaderLibrary& instance()
		{
			static ShaderLibrary instance;
			return instance;
		}
		void Add(const std::string& name, const std::shared_ptr<Shader>& shader);
		bool Add(const std::shared_ptr<Shader>& shader);

		std::shared_ptr<Shader> Load(const std::string& filepath);
		std::shared_ptr<Shader> Load(const std::string& name, const std::string& filepath);

		std::shared_ptr<Shader> Get(const std::string& name);

		bool Exists(const std::string& name) const;
	private:
		ShaderLibrary();
		std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders { };
	};
	
}