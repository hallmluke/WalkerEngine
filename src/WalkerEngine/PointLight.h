#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

class Shader;

class PointLight {
public:
	PointLight(glm::vec3 pos);
	void DrawDebug(Shader& shader);
	void ControlWindow();

	const char* name;

	glm::vec3 position;
	float ambientIntensity;
	float diffuseIntensity;
	float specularIntensity;

	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;

	float debugDrawSize = 0.2f;
	bool drawDebugEnabled = true;

	unsigned int VBO, VAO;

private:
	bool initialized = false;
	void InitBuffers();
};