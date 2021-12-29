#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class PointLight;
class DirectionalLight;

class Shader {
public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
	void use();

	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4(const std::string& name, glm::mat4 value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	void setVec3(const std::string& name, glm::vec3 value) const;
	void setPointLightProperties(PointLight light) const;
	void setDirectionalLightProperties(DirectionalLight light) const;
private:
	void checkCompileErrors(GLuint shader, std::string type);
};

#endif