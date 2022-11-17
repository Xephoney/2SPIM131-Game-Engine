#pragma once

#include <unordered_map>
#include "Shader.h"
#include <glm/glm.hpp>

#include "Engine/Log.h"


namespace Engine
{

    class ShaderCoordinator
    {
    protected:
        Shader* Fallback{ nullptr };
        Shader* Skybox{ nullptr };
        std::unordered_map<std::string, Shader*> m_Shaders;
        ShaderCoordinator() { initializeShaders(); }

    public:
        static ShaderCoordinator& getInstance();

        Shader* fetchSkyboxShader()
        {
            return Skybox;
        }
        Shader* fetchShader(const std::string& name) const
        {
            auto iter = m_Shaders.begin();
            iter = m_Shaders.find(name);
            if (iter == m_Shaders.end())
            {
                return Fallback;
            }
            else
            {
                return iter->second;
            }
        }
        Shader* loadShader(const std::string& shaderName)
        {
            auto iter = m_Shaders.find(shaderName);
            if (iter != m_Shaders.end())
                return (*iter).second;

            m_Shaders[shaderName] = new Shader(shaderName);
            return m_Shaders[shaderName];
        }
        bool updateShaderUniformVec3(const std::string& shaderName, const std::string& uniformName, const glm::vec3& value)
        {
            auto iter = m_Shaders.find(shaderName);
            if (iter == m_Shaders.end())
            {
                ENGINE_LOG_ERROR("Could not update Shader Uniform {0} on shader: {1}",uniformName.c_str(), shaderName.c_str());
                return false;
            }
            m_Shaders[shaderName]->Bind();
            m_Shaders[shaderName]->SetUniform3f(uniformName, value);
            return true;
        }
        void operator=(const ShaderCoordinator&) = delete;
        void CameraUpdate(glm::mat4 v, glm::mat4 p)
        {
            
            for (const auto& a : m_Shaders)
            {
                a.second->Bind();
                a.second->SetUniformMatrix4fv("view", v);
                a.second->SetUniformMatrix4fv("projection", p);
            }
        }

        void UpdateShaders(const glm::mat4& V, const glm::mat4& P)
        {
            UpdateShaders(P * V);
        }
        void UpdateShaders(const glm::mat4& VPM)
        {
            for (const auto& a : m_Shaders)
            {
                a.second->Bind();
                a.second->SetUniformMatrix4fv("u_view_projection", VPM);
            }
        }

    private:
        ShaderCoordinator ShaderCoordiantor(ShaderCoordinator const&) = delete;
        void initializeShaders()
        {
            Fallback = loadShader("plainshader");
            Skybox = loadShader("skybox");
            loadShader("blinn_phongshader");
            loadShader("blinn_phongtextureshader");
            loadShader("unlit_texture");


            m_Shaders.erase("skybox");

        }

    };

    inline ShaderCoordinator& ShaderCoordinator::getInstance()
    {
        static ShaderCoordinator INSTANCE;
        return INSTANCE;
    }
}
