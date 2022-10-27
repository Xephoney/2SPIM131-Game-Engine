#pragma once
#include <string>
#include <glm/glm.hpp>
#include <unordered_map>

class Shader 
{
public:
    // Constructor generates the shader on the fly
    Shader(const std::string& shaderPath);
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    ///Use the current shader
    void Bind() const;
    void Unbind() const;


    void SetUniform1i(const std::string& name, const int& data);
    void SetUniform1f(const std::string& name, const float& data);
    void SetUniform3f(const std::string& name, const glm::vec3& data);
    void SetUniform4f(const std::string& name, const glm::vec4& data);
    void SetUniformMatrix4fv(const std::string& name, const glm::mat4& matrix);
    ///Returns the program number for this shader
    uint32_t getProgram() const;

    int GetUniformLocation(const std::string& name);

private:
    ///The int OpenGL gives as a reference to this shader
    uint32_t m_RenderID;
    std::string m_filePath;
    std::string shaderNavn;
    std::unordered_map<std::string, int> m_UniformLocationCache;

    void CompileShader(const std::string& vertexPath, const std::string& fragmentPath);
    void CompileShaderFromFile(const std::string& vertexPath, const std::string& fragmentPath);
};