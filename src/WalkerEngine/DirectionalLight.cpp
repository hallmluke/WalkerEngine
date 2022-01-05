#include "DirectionalLight.h"
#include "Cube.h"
#include "Shader.h"
#include "Model.h"
#include "imgui\imgui.h";

DirectionalLight::DirectionalLight(glm::vec3 dir, bool shadowMapEnabled) : direction(dir)
{
	name = "Directional Light";
	ambientIntensity = 0.1f;
	diffuseIntensity = 0.8f;
	specularIntensity = 1.0f;

	DirectionalLight::shadowMapEnabled = shadowMapEnabled;
}

void DirectionalLight::DrawDebug(Shader& shader)
{
	if (!initialized) {
		InitBuffers();
	}

	glBindVertexArray(VAO);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, direction * -debugDistance);
	model = glm::scale(model, glm::vec3(debugDrawSize));
	shader.setMat4("model", model);

	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void DirectionalLight::ControlWindow()
{
	if (ImGui::Begin(name)) {
		ImGui::Text("Direction");
		ImGui::SliderFloat("X", &direction.x, -1.0f, 1.0f);
		ImGui::SliderFloat("Y", &direction.y, -1.0f, 1.0f);
		ImGui::SliderFloat("Z", &direction.z, -1.0f, 1.0f);

		ImGui::Text("Intensity");
		ImGui::SliderFloat("Ambient Intensity", &ambientIntensity, 0.0f, 0.5f);
		ImGui::SliderFloat("Diffuse Intensity", &diffuseIntensity, 0.0f, 1.0f);
		ImGui::SliderFloat("Specular Intensity", &specularIntensity, 0.0f, 1.2f);

		ImGui::Text("Shadow Mapping");
		//ImGui::Checkbox("Enabled", &shadowMapEnabled);
		//ImGui::Text("Bias");
		ImGui::SliderFloat("Minimum Shadow Bias", &minimumShadowBias, 0.0001, 0.025);
		//ImGui::SliderFloat("Shadow bias factor", &shadowBiasFactor, 0.001, 0.05);

		ImGui::Text("Debug");
		ImGui::Checkbox("Draw", &drawDebugEnabled);
		ImGui::SliderFloat("Light scale", &debugDrawSize, 0.01f, 3.0f);
		ImGui::SliderFloat("Distance", &debugDistance, 1.0f, 40.0f);
	}
	ImGui::End();
}

glm::mat4 DirectionalLight::GetLightSpaceMatrix()
{
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	lightProjection = glm::ortho(-projectionSize, projectionSize, -projectionSize, projectionSize, nearPlane, farPlane);
	lightView = glm::lookAt(direction * -debugDistance, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;
	return lightSpaceMatrix;
}

void DirectionalLight::GenerateShadowMap(Shader& shader, Model& model)
{
	if (!shadowMapInitialized) {
		InitShadowMap();
	}
	// Get current viewport size to reset after
	GLint m_viewport[4];
	glGetIntegerv(GL_VIEWPORT, m_viewport);

	shader.use();
	shader.setMat4("lightSpaceMatrix", GetLightSpaceMatrix());

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	//shader.setMat4("model", model.transform);
	model.Draw(shader);

	// Reset framebuffer, viewport
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void DirectionalLight::InitBuffers()
{
	vector<float> vertices = Cube::getUnindexedVertices();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Cube::CubeVertex), &vertices[0], GL_STATIC_DRAW);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	initialized = true;
}

void DirectionalLight::InitShadowMap()
{
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	shadowMapInitialized = true;
}
