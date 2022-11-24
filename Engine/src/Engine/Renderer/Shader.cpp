#include "engpch.h"

#include "Shader.h"

#include <fstream> 

#include "Engine/Log.h"
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

namespace Engine
{
	
}

Shader::Shader(const std::string& shaderPath)
{
	std::string vert = "C:/dev/2SPIM131/Engine/src/Engine/Renderer/Shaders/" + shaderPath + ".vert";
	std::string frag = "C:/dev/2SPIM131/Engine/src/Engine/Renderer/Shaders/" + shaderPath + ".frag";
	CompileShaderFromFile(vert, frag);
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
	CompileShader(vertexPath, fragmentPath);
	shaderNavn = "SHADER";
}

Shader::~Shader()
{
	glDeleteProgram(m_RenderID);
}

void Shader::Bind() const
{
	glUseProgram(m_RenderID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::SetUniform1i(const std::string& name, const int& data) const
{

}

void Shader::SetUniform1f(const std::string& name, const float& data)const
{

}

void Shader::SetUniform3f(const std::string& name, const glm::vec3& data)const
{
}

void Shader::SetUniform4f(const std::string& name, const glm::vec4& data)const
{
}

void Shader::SetUniformMatrix4fv(const std::string& name, const glm::mat4& matrix ) 
{
	int location = GetUniformLocation(name);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

uint32_t Shader::getProgram() const
{
	return m_RenderID;
}

int Shader::GetUniformLocation(const std::string& name) 
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];
	int location = glGetUniformLocation(m_RenderID, name.c_str());
	if (location == -1)
	{
		ENGINE_CORE_ASSERT(false, "UNIFORM LOCATION \"{0}\" DOES NOT EXIST", name)
		return -1;
	}

	m_UniformLocationCache[name] = location;
	return location;
}

void Shader::CompileShader(const std::string& vertexPath, const std::string& fragmentPath)
{
	// Create an empty vertex shader handle
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Send the vertex shader source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	const GLchar* source = vertexPath.c_str();
	glShaderSource(vertexShader, 1, &source, 0);

	// Compile the vertex shader
	glCompileShader(vertexShader);

	GLint isCompiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

		// We don't need the shader anymore.
		glDeleteShader(vertexShader);

		// Use the infoLog as you see fit.
		ENGINE_LOG_ERROR("FAILED TO COMPILE VERTEX SHADER {0}", vertexPath);
		ENGINE_LOG_ERROR("{0}", infoLog.data());
		ENGINE_CORE_ASSERT(false, "Vertex Shader compilation failure");
		return;
	}

	// Create an empty fragment shader handle
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Send the fragment shader source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	source = fragmentPath.c_str();
	glShaderSource(fragmentShader, 1, &source, 0);

	// Compile the fragment shader
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

		// We don't need the shader anymore.
		glDeleteShader(fragmentShader);
		// Either of them. Don't leak shaders.
		glDeleteShader(vertexShader);

		// Use the infoLog as you see fit.
		ENGINE_LOG_ERROR("FAILED TO COMPILE FRAGMENT SHADER {0}", fragmentPath)
		ENGINE_LOG_ERROR("{0}", infoLog.data());
		ENGINE_CORE_ASSERT(false, "Fragment Shader compilation failure")
		// In this simple program, we'll just leave
		return;
	}

	// Vertex and fragment shaders are successfully compiled.
	// Now time to link them together into a program.
	// Get a program object.
	m_RenderID = glCreateProgram();
	GLuint program = m_RenderID;
	// Attach our shaders to our program
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	// Link our program
	glLinkProgram(program);

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		// We don't need the program anymore.
		glDeleteProgram(program);
		// Don't leak shaders either.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		// Use the infoLog as you see fit.
		ENGINE_LOG_ERROR("{0}", infoLog.data());
		ENGINE_CORE_ASSERT(false, "Shader linking failure")
		// In this simple program, we'll just leave
		return;
	}

	// Always detach shaders after a successful link.
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);
}

void Shader::CompileShaderFromFile(const std::string& vertexPath, const std::string& fragmentPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	// Open files and check for errors
	vShaderFile.open(vertexPath);
	if (!vShaderFile)
	{
		ENGINE_LOG_ERROR("FAILED TO OPEN .VERT {0}", vertexPath);
	}

	fShaderFile.open(fragmentPath);
	if (!fShaderFile)
		ENGINE_LOG_ERROR("FAILED TO OPEN .FRAG {0}", vertexPath);

	std::stringstream vShaderStream, fShaderStream;
	// Read file's buffer contents into streams
	vShaderStream << vShaderFile.rdbuf();
	fShaderStream << fShaderFile.rdbuf();
	// close file handlers
	vShaderFile.close();
	fShaderFile.close();
	// Convert stream into string
	vertexCode = vShaderStream.str();
	fragmentCode = fShaderStream.str();

	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();

	// 2. Compile shaders
	GLuint vertex, fragment;
	GLint success;
	std::vector<GLchar> infoLog(512);
	// Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, nullptr);
	glCompileShader(vertex);
	// Print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, nullptr, infoLog.data());
		ENGINE_LOG_ERROR("FAILED TO COMPILE VERTEX SHADER {0}", fragmentPath);
		ENGINE_LOG_ERROR("{0}", infoLog.data());
		ENGINE_CORE_ASSERT(false, "Fragment Shader compilation failure")
	}
	// Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	glCompileShader(fragment);
	// Print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, nullptr, infoLog.data());
		ENGINE_LOG_ERROR("FAILED TO COMPILE FRAGMENT SHADER {0}", fragmentPath);
		ENGINE_LOG_ERROR("{0}", infoLog.data());
		ENGINE_CORE_ASSERT(false, "Fragment Shader compilation failure")
	}
	// Shader Program linking
	m_RenderID = glCreateProgram();
	glAttachShader(m_RenderID, vertex);
	glAttachShader(m_RenderID, fragment);
	glLinkProgram(m_RenderID);
	// Print linking errors if any
	glGetProgramiv(m_RenderID, GL_LINK_STATUS, &success);

	//making nice output name for printing:
	std::string shadername{ vertexPath };
	shadername.resize(shadername.size() - 5); //deleting ".vert"

	if (!success)
	{
		glGetProgramInfoLog(m_RenderID, 512, nullptr, infoLog.data());
		ENGINE_LOG_ERROR("{0}", infoLog.data());
		ENGINE_CORE_ASSERT(false, "Shader linking failure")
	}
	else
	{
		ENGINE_LOG_INFO("GLSL shader " + shadername + " was successfully compiled");
	}
	// Delete the shaders as they're linked into our program now and no longer needed
	// The shader program is now on the GPU and we reference it by using the mProgram variable
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}
