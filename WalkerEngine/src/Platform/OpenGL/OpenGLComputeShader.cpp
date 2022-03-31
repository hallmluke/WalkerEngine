#include "walkerpch.h"
#include "OpenGLComputeShader.h"

#include <fstream>
#include <filesystem>
#include <GLM/gtc/type_ptr.hpp>

namespace Walker {

	OpenGLComputeShader::OpenGLComputeShader(const std::string& name, const std::string& path)
	{
        std::string code;
        std::ifstream shaderFile;
        shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            shaderFile.open(path);
            std::stringstream shaderStream;

            shaderStream << shaderFile.rdbuf();

            shaderFile.close();

            code = shaderStream.str();
        }
        catch (std::ifstream::failure& e)
        {
            W_CORE_ERROR("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ \n Compute Path: {0}", path);
        }

        const char* shaderCode = code.c_str();

        uint32_t compute;

        compute = glCreateShader(GL_COMPUTE_SHADER);
        glShaderSource(compute, 1, &shaderCode, NULL);
        glCompileShader(compute);
        CheckCompileErrors(compute, "COMPUTE");

        m_Id = glCreateProgram();
        glAttachShader(m_Id, compute);
        glLinkProgram(m_Id);
        CheckCompileErrors(m_Id, "PROGRAM");

        glDeleteShader(compute);
        
	}

    OpenGLComputeShader::~OpenGLComputeShader()
    {
        glDeleteProgram(m_Id);
    }

    void OpenGLComputeShader::Bind() const
    {
        glUseProgram(m_Id);
    }

    void OpenGLComputeShader::Unbind() const
    {
        glUseProgram(0);
    }

    void OpenGLComputeShader::Dispatch(uint32_t numGroupsX, uint32_t numGroupsY, uint32_t numGroupsZ)
    {
        glDispatchCompute(numGroupsX, numGroupsY, numGroupsZ);
    }

    void OpenGLComputeShader::Barrier()
    {
        // TODO: Support multiple barrier bit types
        //glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT & GL_TEXTURE_FETCH_BARRIER_BIT);
        glMemoryBarrier(GL_ALL_BARRIER_BITS);
    }

    void OpenGLComputeShader::SetBool(const std::string& name, bool value)
    {
        glUniform1i(GetUniformLocation(name), (int)value);
    }

    void OpenGLComputeShader::SetInt(const std::string& name, int value)
    {
        glUniform1i(GetUniformLocation(name), value);
    }

    void OpenGLComputeShader::SetFloat(const std::string& name, float value)
    {
        glUniform1f(GetUniformLocation(name), value);
    }

    void OpenGLComputeShader::SetMat4(const std::string& name, glm::mat4 value)
    {
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
    }

    void OpenGLComputeShader::SetVec3(const std::string& name, float x, float y, float z)
    {
        glUniform3f(GetUniformLocation(name), x, y, z);
    }

    void OpenGLComputeShader::SetVec3(const std::string& name, glm::vec3 value)
    {
        glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
    }

    void OpenGLComputeShader::CheckCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                W_CORE_ERROR("ERROR::SHADER_COMPILATION_ERROR of type: {0} \n {1}", type, infoLog);
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                W_CORE_ERROR("ERROR::PROGRAM_LINKING_ERROR of type: {0} \n {1}", type, infoLog);
            }
        }
    }

    GLint OpenGLComputeShader::GetUniformLocation(const std::string& name) const
    {
        if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
            return m_UniformLocationCache[name];
        }

        GLint location = glGetUniformLocation(m_Id, name.c_str());
        m_UniformLocationCache[name] = location;
        return location;
    }
}