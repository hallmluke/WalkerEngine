#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

class Shader;
class Model;

class DirectionalLight {
public:
	DirectionalLight(glm::vec3 dir, bool shadowMapEnabled = true);
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

	// Shadow mapping
	bool shadowMapEnabled = true;
	unsigned int depthMapFBO;
	unsigned int depthMap;
	float nearPlane = 0.1f;
	float farPlane = 70.0f;
	float projectionSize = 20.0f;
	glm::mat4 GetLightSpaceMatrix();
	float minimumShadowBias = 0.001f;
	float shadowBiasFactor = 0.005f;
	void GenerateShadowMap(Shader& shader, Model& model);

private:
	bool initialized = false;
	void InitBuffers();

	// Shadow mapping
	const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
	bool shadowMapInitialized = false;
	void InitShadowMap();
};