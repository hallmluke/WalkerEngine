#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include "Shader.h"

class Model;
class Camera;

class DirectionalLight {
public:
	DirectionalLight(glm::vec3 dir, bool shadowMapEnabled = true);
	void DrawDebug(Shader& shader);
	void ControlWindow();

	const char* name;

	glm::vec3 direction;
	glm::vec3 color;

	float ambientIntensity;
	float diffuseIntensity;
	float specularIntensity;

	float debugDrawSize = 0.2f;
	bool drawDebugEnabled = true;
	float debugDistance = 20.0f;

	unsigned int VBO, VAO;

	// Shadow mapping
	bool shadowMapEnabled = true;
	bool cascadedShadowMapping = true;
	unsigned int depthMapFBO;
	unsigned int depthMap;
	float nearPlane = 0.1f;
	float farPlane = 70.0f;
	float projectionSize = 20.0f;
	glm::mat4 GetLightSpaceMatrix();
	glm::mat4 GetLightSpaceMatrix(Camera& camera, const float nearPlane, const float farPlane);
	std::vector<glm::mat4> GetLightSpaceMatrices(Camera& camera);
	float minimumShadowBias = 0.001f;
	float shadowBiasFactor = 0.005f;
	void GenerateShadowMap(Shader& shader, Model& model, Camera& camera);
	void GenerateCascadedShadowMap(Model& model, Camera& camera);
	std::vector<float> GetShadowCascadeLevels(float cameraFarPlane);

private:
	unsigned int matricesUBO;
	void InitUBO();
	bool initialized = false;
	void InitBuffers();

	// Shadow mapping
	const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
	bool shadowMapInitialized = false;
	void InitShadowMap();
	void InitCascadedShadowMaps();
	int shadowCascadeLevels = 4;
	Shader cascadedShader = Shader("Shaders/depth_shader_cascades.vert", "Shaders/depth_shader.frag", "Shaders/depth_shader_cascades.geom");
};