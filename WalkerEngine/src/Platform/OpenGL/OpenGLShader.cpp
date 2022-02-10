#include "walkerpch.h"

#include <fstream>
#include "OpenGLShader.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "Camera.h"

namespace Walker {

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::string geometryCode;

        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        std::ifstream gShaderFile;

        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;

            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            // close file handlers
            vShaderFile.close();
            fShaderFile.close();

            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();

            // if geometry shader path is present, also load a geometry shader
            if (!geometryPath.empty())
            {
                gShaderFile.open(geometryPath);
                std::stringstream gShaderStream;
                gShaderStream << gShaderFile.rdbuf();
                gShaderFile.close();
                geometryCode = gShaderStream.str();
            }
        }
        catch (std::ifstream::failure& e)
        {
            W_CORE_ERROR("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ \n VertexPath: {0} FragmentPath: {1}", vertexPath, fragmentPath);
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        // 2. compile shaders
        unsigned int vertex, fragment;

        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        CheckCompileErrors(vertex, "VERTEX");

        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        CheckCompileErrors(fragment, "FRAGMENT");

        // if geometry shader is given, compile geometry shader
        unsigned int geometry;
        if (!geometryPath.empty())
        {
            const char* gShaderCode = geometryCode.c_str();
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &gShaderCode, NULL);
            glCompileShader(geometry);
            CheckCompileErrors(geometry, "GEOMETRY");
        }

        // shader Program
        m_Id = glCreateProgram();
        glAttachShader(m_Id, vertex);
        glAttachShader(m_Id, fragment);

        if (!geometryPath.empty()) {
            glAttachShader(m_Id, geometry);
        }
        glLinkProgram(m_Id);
        CheckCompileErrors(m_Id, "PROGRAM");


        // delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if (!geometryPath.empty()) {
            glDeleteShader(geometry);
        }
    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_Id);
    }

    void OpenGLShader::Bind() const
    {
        glUseProgram(m_Id);
    }

    void OpenGLShader::Unbind() const
    {
        glUseProgram(0);
    }

    void OpenGLShader::SetBool(const std::string& name, bool value)
    {
        glUniform1i(GetUniformLocation(name), (int)value);
    }

    void OpenGLShader::SetInt(const std::string& name, int value)
    {
        glUniform1i(GetUniformLocation(name), value);
    }

    void OpenGLShader::SetFloat(const std::string& name, float value)
    {
        glUniform1f(GetUniformLocation(name), value);
    }

    void OpenGLShader::SetMat4(const std::string& name, glm::mat4 value)
    {
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
    }

    void OpenGLShader::SetVec3(const std::string& name, float x, float y, float z)
    {
        glUniform3f(GetUniformLocation(name), x, y, z);
    }

    void OpenGLShader::SetVec3(const std::string& name, glm::vec3 value)
    {
        glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
    }

    /*void OpenGLShader::SetPointLightProperties(PointLight light)
    {
        /*SetVec3("pointLight.position", light.position);
        SetVec3("pointLight.ambient", glm::vec3(light.ambientIntensity));
        SetVec3("pointLight.diffuse", glm::vec3(light.diffuseIntensity));
        SetVec3("pointLight.specular", glm::vec3(light.specularIntensity));
        SetFloat("pointLight.constant", light.constantAttenuation);
        SetFloat("pointLight.linear", light.linearAttenuation);
        SetFloat("pointLight.quadratic", light.quadraticAttenuation);

        if (light.shadowMapEnabled) {
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_CUBE_MAP, light.depthCubeMap);
            SetInt("pointLight.depthMap", 4);
            SetFloat("pointLight.far_plane", light.farPlane);
            SetFloat("pointLight.bias", light.bias);
        }

        SetBool("debugShadow", light.debugShadow);
    }

    void OpenGLShader::SetPointLightProperties(std::vector<PointLight*> lights)
    {
        /*int maxLights = 64;
        int maxedLights = maxLights;

        if (lights.size() < maxLights) {
            maxedLights = lights.size();
        }

        for (int i = 0; i < maxedLights; i++) {
            std::string lightPrefix = "lights[" + std::to_string(i) + "]";
            SetVec3(lightPrefix + ".position", lights[i]->position);
            SetVec3(lightPrefix + ".ambient", glm::vec3(lights[i]->ambientIntensity));
            SetVec3(lightPrefix + ".diffuse", glm::vec3(lights[i]->diffuseIntensity));
            SetVec3(lightPrefix + ".specular", glm::vec3(lights[i]->specularIntensity));
            SetFloat(lightPrefix + ".constant", lights[i]->constantAttenuation);
            SetFloat(lightPrefix + ".linear", lights[i]->linearAttenuation);
            SetFloat(lightPrefix + ".quadratic", lights[i]->quadraticAttenuation);

            if (lights[i]->shadowMapEnabled) {
                glActiveTexture(GL_TEXTURE6);
                glBindTexture(GL_TEXTURE_CUBE_MAP, lights[i]->depthCubeMap);
                SetInt(lightPrefix + ".depthMap", 6);
                SetFloat(lightPrefix + ".far_plane", lights[i]->farPlane);
                SetFloat(lightPrefix + ".bias", lights[i]->bias);
            }
        }

        SetInt("numberOfLights", maxedLights);
    }*/

    void OpenGLShader::SetDirectionalLightProperties(DirectionalLight light, Camera camera)
    {
        SetVec3("dirLight.direction", light.GetDirection());
        SetVec3("dirLight.color", light.GetColor());
        SetVec3("dirLight.ambient", glm::vec3(light.GetAmbientIntensity()));
        SetVec3("dirLight.diffuse", glm::vec3(light.GetDiffuseIntensity()));
        SetVec3("dirLight.specular", glm::vec3(light.GetSpecularIntensity()));

        /*if (light.shadowMapEnabled) {
            if (light.cascadedShadowMapping) {
                //SetMat4("dirLight.lightSpaceMatrix", light.GetLightSpaceMatrix(camera));
                //SetFloat("dirLight.shadowBias", light.minimumShadowBias);
                std::vector<float> cascadeDistances = light.GetShadowCascadeLevels(camera.farPlane);
                SetMat4("view", camera.GetViewMatrix());
                SetFloat("farPlane", camera.farPlane);
                SetInt("cascadeCount", cascadeDistances.size());
                for (size_t i = 0; i < cascadeDistances.size(); ++i)
                {
                    SetFloat("cascadePlaneDistances[" + std::to_string(i) + "]", cascadeDistances[i]);
                }
                glActiveTexture(GL_TEXTURE5);
                glBindTexture(GL_TEXTURE_2D_ARRAY, light.depthMap);
                SetInt("shadowMap", 5);
            }
            else {
                SetMat4("dirLight.lightSpaceMatrix", light.GetLightSpaceMatrix());
                SetFloat("dirLight.shadowBias", light.minimumShadowBias);

                glActiveTexture(GL_TEXTURE5);
                glBindTexture(GL_TEXTURE_2D, light.depthMap);
                SetInt("dirLight.shadowMap", 5);
            }
        }*/
    }

    void OpenGLShader::CheckCompileErrors(GLuint shader, std::string type)
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

    GLint OpenGLShader::GetUniformLocation(const std::string& name) const
    {
        if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
            return m_UniformLocationCache[name];
        }

        GLint location = glGetUniformLocation(m_Id, name.c_str());
        m_UniformLocationCache[name] = location;
        return location;
    }

}