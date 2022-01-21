#include "Shader.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "Camera.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
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
        if (geometryPath != nullptr)
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
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        std::cout << "Vert: " << vertexPath << std::endl;
        std::cout << "Frag: " << fragmentPath << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. compile shaders
    unsigned int vertex, fragment;

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // if geometry shader is given, compile geometry shader
    unsigned int geometry;
    if (geometryPath != nullptr)
    {
        const char* gShaderCode = geometryCode.c_str();
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gShaderCode, NULL);
        glCompileShader(geometry);
        checkCompileErrors(geometry, "GEOMETRY");
    }

    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);

    if (geometryPath != nullptr) {
        glAttachShader(ID, geometry);
    }
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (geometryPath != nullptr) {
        glDeleteShader(geometry);
    }
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(GetUniformLocation(name), (int)value);
}
// ------------------------------------------------------------------------
void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(GetUniformLocation(name), value);
}
// ------------------------------------------------------------------------
void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::setMat4(const std::string& name, glm::mat4 value) const
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(GetUniformLocation(name), x, y, z);
}

void Shader::setVec3(const std::string& name, glm::vec3 value) const
{
	glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
}

void Shader::setPointLightProperties(PointLight light) const
{
    setVec3("pointLight.position", light.position);
    setVec3("pointLight.ambient", glm::vec3(light.ambientIntensity));
    setVec3("pointLight.diffuse", glm::vec3(light.diffuseIntensity));
    setVec3("pointLight.specular", glm::vec3(light.specularIntensity));
    setFloat("pointLight.constant", light.constantAttenuation);
    setFloat("pointLight.linear", light.linearAttenuation);
    setFloat("pointLight.quadratic", light.quadraticAttenuation);

    if (light.shadowMapEnabled) {
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_CUBE_MAP, light.depthCubeMap);
        setInt("pointLight.depthMap", 4);
        setFloat("pointLight.far_plane", light.far);
        setFloat("pointLight.bias", light.bias);
    }

    setBool("debugShadow", light.debugShadow);
}

void Shader::setPointLightProperties(std::vector<PointLight*> lights) const
{
    int maxedLights = 0;
    int maxLights = 64;

    if (lights.size() < maxLights) {
        maxedLights = lights.size();
    }
    else {
        maxedLights = maxLights;
    }

    for (int i = 0; i < maxedLights; i++) {
        std::string lightPrefix = "lights[" + std::to_string(i) + "]";
        setVec3(lightPrefix + ".position", lights[i]->position);
        setVec3(lightPrefix + ".ambient", glm::vec3(lights[i]->ambientIntensity));
        setVec3(lightPrefix + ".diffuse", glm::vec3(lights[i]->diffuseIntensity));
        setVec3(lightPrefix + ".specular", glm::vec3(lights[i]->specularIntensity));
        setFloat(lightPrefix + ".constant", lights[i]->constantAttenuation);
        setFloat(lightPrefix + ".linear", lights[i]->linearAttenuation);
        setFloat(lightPrefix + ".quadratic", lights[i]->quadraticAttenuation);

        if (lights[i]->shadowMapEnabled) {
            glActiveTexture(GL_TEXTURE6);
            glBindTexture(GL_TEXTURE_CUBE_MAP, lights[i]->depthCubeMap);
            setInt(lightPrefix + ".depthMap", 6);
            setFloat(lightPrefix + ".far_plane", lights[i]->far);
            setFloat(lightPrefix + ".bias", lights[i]->bias);
        }
    }
    
    setInt("numberOfLights", maxedLights);
}

void Shader::setDirectionalLightProperties(DirectionalLight light, Camera camera) const
{
    setVec3("dirLight.direction", light.direction);
    setVec3("dirLight.color", light.color);
    setVec3("dirLight.ambient", glm::vec3(light.ambientIntensity));
    setVec3("dirLight.diffuse", glm::vec3(light.diffuseIntensity));
    setVec3("dirLight.specular", glm::vec3(light.specularIntensity));

    if (light.shadowMapEnabled) {
        if (light.cascadedShadowMapping) {
            //setMat4("dirLight.lightSpaceMatrix", light.GetLightSpaceMatrix(camera));
            //setFloat("dirLight.shadowBias", light.minimumShadowBias);
            std::vector<float> cascadeDistances = light.GetShadowCascadeLevels(camera.farPlane);
            setMat4("view", camera.GetViewMatrix());
            setFloat("farPlane", camera.farPlane);
            setInt("cascadeCount", cascadeDistances.size());
            for (size_t i = 0; i < cascadeDistances.size(); ++i)
            {
                setFloat("cascadePlaneDistances[" + std::to_string(i) + "]", cascadeDistances[i]);
            }
            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_2D_ARRAY, light.depthMap);
            setInt("shadowMap", 5);
        }
        else {
            setMat4("dirLight.lightSpaceMatrix", light.GetLightSpaceMatrix());
            setFloat("dirLight.shadowBias", light.minimumShadowBias);

            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_2D, light.depthMap);
            setInt("dirLight.shadowMap", 5);
        }
    }
}

void Shader::checkCompileErrors(GLuint shader, std::string type)
{
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

GLint Shader::GetUniformLocation(const std::string& name) const
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
        return m_UniformLocationCache[name];
    }

    GLint location = glGetUniformLocation(ID, name.c_str());
    m_UniformLocationCache[name] = location;
    return location;
}
