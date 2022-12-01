#include <engpch.h>

#include "OpenGLShader.h"

#include <fstream>

#include "Engine/Core.h"
#include "Engine/Log.h"
#include "glm/gtc/type_ptr.hpp"

namespace Engine
{
	namespace Utils
	{
		static auto ShaderTypeFromString(const std::string& type) -> GLenum
		{
			if (type == "vertex")
				return GL_VERTEX_SHADER;
			if (type == "fragment" || type == "pixel")
				return GL_FRAGMENT_SHADER;

			ENGINE_CORE_ASSERT(false, "Unknown shader type!");
			return 0;
		}

		static auto GLShaderStageToString(GLenum stage) -> const char*
		{
			switch (stage)
			{
				case GL_VERTEX_SHADER:		return "GL_VERTEX_SHADER";
				case GL_FRAGMENT_SHADER:	return "GL_FRAGMENT_SHADER";
				case GL_COMPUTE_SHADER:		return "GL_COMPUTE_SHADER";
			default: 
				ENGINE_CORE_ASSERT(false, "");
			}
			return "";
		}
	}


	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		m_FilePath = filepath;
		const std::string source = ReadFile(filepath);
		const auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		// Extract name from filepath
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	auto OpenGLShader::Bind() const -> void
	{
		glUseProgram(m_RendererID);
	}

	auto OpenGLShader::Unbind() const -> void
	{
		glUseProgram(0);
	}

	auto OpenGLShader::SetInt(const std::string& name, int value) -> void
	{
		UploadUniformInt(name, value);
	}

	auto OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count) -> void
	{
		UploadUniformIntArray(name, values, count);
	}

	auto Engine::OpenGLShader::SetFloat(const std::string& name, float value) -> void
	{
		UploadUniformFloat(name, value);
	}

	auto OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value) -> void
	{
		UploadUniformFloat2(name, value);
	}

	auto OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value) -> void
	{
		UploadUniformFloat3(name, value);
	}

	auto OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value) -> void
	{
		UploadUniformFloat4(name, value);
	}

	auto OpenGLShader::SetMat3(const std::string& name, const glm::mat3& value) -> void
	{
		UploadUniformMat3(name, value);
	}

	auto OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value) -> void
	{
		UploadUniformMat4(name, value);
	}

	auto OpenGLShader::GetName() const -> const std::string&
	{
		return m_Name;
	}

	auto OpenGLShader::UploadUniformInt(const std::string& name, int value) -> void
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	auto OpenGLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count) -> void
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, count, values);
	}

	auto OpenGLShader::UploadUniformFloat(const std::string& name, float value) -> void
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	auto OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value) -> void
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	auto OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value) -> void
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	auto OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value) -> void
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	auto OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix) -> void
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	auto OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) -> void
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	auto OpenGLShader::ReadFile(const std::string& filepath) -> std::string
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
			}
			else
			{
				ENGINE_LOG_ERROR("Could not read from file '{0}'", filepath);
			}
		}
		else
		{
			ENGINE_LOG_ERROR("Could not open file '{0}'", filepath);
		}

		return result;
	}

	auto OpenGLShader::PreProcess(const std::string& source) -> std::unordered_map<GLenum, std::string>
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0); //Start of shader type declaration line
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
			ENGINE_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
			std::string type = source.substr(begin, eol - begin);
			ENGINE_CORE_ASSERT(Utils::ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
			ENGINE_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line

			shaderSources[Utils::ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	auto OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources) -> void
	{
		GLuint program = glCreateProgram();
		const int a = shaderSources.size();
		std::array<GLenum, 6> glShaderIDs;
		
		uint8_t counter = 0;
		for(auto& kv : shaderSources)
		{
			GLenum Type = kv.first;
			const std::string& Source = kv.second;

			GLuint shader = glCreateShader(Type);

			const GLchar* sourceCSTR = Source.c_str();
			glShaderSource(shader, 1, &sourceCSTR, 0);
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if(isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				ENGINE_LOG_ERROR("{0}", infoLog.data());
				ENGINE_CORE_ASSERT(false, " || SHADER COMPILATION FAILED ||");
				break;
			}

			glAttachShader(program, shader);
			glShaderIDs[counter++] = shader;
		}
		glLinkProgram(program);
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{

			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
			glDeleteProgram(program);

			for(int i = 0; i < counter; i++)
			{
				glDeleteShader(glShaderIDs[i]);
			}

			ENGINE_LOG_ERROR("{0}", infoLog.data())
				ENGINE_CORE_ASSERT(false, "|| SHADER LINK FAILURE! ||")
				return;
		}

		for (int i = 0; i < counter; i++)
		{
			glDetachShader(program, glShaderIDs[i]);
		}

		m_RendererID = program;
	}

	auto OpenGLShader::CreateProgram() -> void
	{
		GLuint program = glCreateProgram();

		std::vector<GLuint> shaderIDs;

		glLinkProgram(program);
		GLint isLinked;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
			ENGINE_LOG_ERROR("Shader linking failed ({0}):\n{1}", m_FilePath, infoLog.data());

			glDeleteProgram(program);

			for (auto id : shaderIDs)
				glDeleteShader(id);
		}

		for (auto id : shaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}

		m_RendererID = program;
	}
}
