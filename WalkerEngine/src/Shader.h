#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class PointLight;
class DirectionalLight;
class Camera;

class Shader {
public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
	//~Shader();

	void use();

	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4(const std::string& name, glm::mat4 value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	void setVec3(const std::string& name, glm::vec3 value) const;
	void setPointLightProperties(PointLight light) const;
	void setPointLightProperties(std::vector<PointLight*> lights) const;
	void setDirectionalLightProperties(DirectionalLight light, Camera camera) const;
private:
	void checkCompileErrors(GLuint shader, std::string type);
	mutable std::unordered_map<std::string, GLint> m_UniformLocationCache;

	GLint GetUniformLocation(const std::string& name) const;
};

#endif