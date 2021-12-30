#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

class Shader;

class DirectionalLight {
public:
	DirectionalLight(glm::vec3 dir);
	void DrawDebug(Shader& shader);
	void ControlWindow();

	const char* name;

	glm::vec3 direction;

	float ambientIntensity;
	float diffuseIntensity;
	float specularIntensity;

	float debugDrawSize = 0.2f;
	bool drawDebugEnabled = true;
	float debugDistance = 20.0f;

	unsigned int VBO, VAO;

private:
	bool initialized = false;
	void InitBuffers();
};