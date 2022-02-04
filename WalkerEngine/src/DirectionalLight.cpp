#include "walkerpch.h"
#include "DirectionalLight.h"
#include "Cube.h"
#include "Renderer/Shader.h"
#include "Model.h"
#include "Camera.h"
#include "imgui\imgui.h";
#include <glm/gtx/string_cast.hpp>

namespace Walker {
	/*
	DirectionalLight::DirectionalLight(glm::vec3 dir, bool shadowMapEnabled) : m_Direction(dir)
	{
		m_Name = "Directional Light";
		m_AmbientIntensity = 0.1f;
		m_DiffuseIntensity = 0.8f;
		m_SpecularIntensity = 1.0f;
		m_Color = glm::vec3(0.8f);

		DirectionalLight::shadowMapEnabled = shadowMapEnabled;

		InitUBO();
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

			ImGui::Text("Color");
			ImGui::ColorPicker4("Color", (float*)&color);

			ImGui::Text("Intensity");
			ImGui::SliderFloat("Ambient Intensity", &ambientIntensity, 0.0f, 0.5f);
			ImGui::SliderFloat("Diffuse Intensity", &diffuseIntensity, 0.0f, 20.0f);
			ImGui::SliderFloat("Specular Intensity", &specularIntensity, 0.0f, 1.2f);

			ImGui::Text("Shadow Mapping");
			//ImGui::Checkbox("Enabled", &shadowMapEnabled);
			//ImGui::Text("Bias");
			ImGui::SliderFloat("Minimum Shadow Bias", &minimumShadowBias, 0.0001, 0.025);
			//ImGui::SliderFloat("Shadow bias factor", &shadowBiasFactor, 0.001, 0.05);

			ImGui::Text("Debug");
			ImGui::Checkbox("Draw", &drawDebugEnabled);
			ImGui::SliderFloat("Light scale", &debugDrawSize, 0.01f, 3.0f);
			ImGui::SliderFloat("Distance", &debugDistance, 0.1f, 80.0f);
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

	glm::mat4 DirectionalLight::GetLightSpaceMatrix(Camera& camera, const float nearPlane, const float farPlane)
	{
		std::vector<glm::vec4> corners = camera.getFrustumCornersWorldSpace(camera.GetProjectionMatrix(16.0f / 9.0f, nearPlane, farPlane), camera.GetViewMatrix());

		glm::vec3 center = glm::vec3(0, 0, 0);
		for (const auto& v : corners)
		{
			center += glm::vec3(v);
		}
		center /= corners.size();
		ImGui::Text(("Center: " + glm::to_string(center)).c_str());
		glm::vec3 centerDirection = center - direction;
		ImGui::Text(("CenterDirection: " + glm::to_string(centerDirection)).c_str());

		const auto lightView = glm::lookAt(
			center - direction,
			center,
			glm::vec3(0.0f, 1.0f, 0.0f)
		);

		float minX = std::numeric_limits<float>::max();
		float maxX = std::numeric_limits<float>::min();
		float minY = std::numeric_limits<float>::max();
		float maxY = std::numeric_limits<float>::min();
		float minZ = std::numeric_limits<float>::max();
		float maxZ = std::numeric_limits<float>::min();
		for (const auto& v : corners)
		{
			const auto trf = lightView * v;
			minX = std::min(minX, trf.x);
			maxX = std::max(maxX, trf.x);
			minY = std::min(minY, trf.y);
			maxY = std::max(maxY, trf.y);
			minZ = std::min(minZ, trf.z);
			maxZ = std::max(maxZ, trf.z);
		}

		ImGui::Text(("Min X: " + std::to_string(minX)).c_str());
		ImGui::Text(("Max X: " + std::to_string(maxX)).c_str());
		ImGui::Text(("Min Y: " + std::to_string(minY)).c_str());
		ImGui::Text(("Max Y: " + std::to_string(maxY)).c_str());
		ImGui::Text(("Min Z: " + std::to_string(minZ)).c_str());
		ImGui::Text(("Max Z: " + std::to_string(maxZ)).c_str());

		if (minZ < 0)
		{
			minZ *= debugDistance;
		}
		else
		{
			minZ /= debugDistance;
		}
		if (maxZ < 0)
		{
			maxZ /= debugDistance;
		}
		else
		{
			if (maxZ < 0.5f) {
				maxZ = 1.0f;
			}
			//maxZ += 0.1;
			maxZ *= debugDistance;
		}

		const glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);

		return lightProjection * lightView;
	}

	std::vector<glm::mat4> DirectionalLight::GetLightSpaceMatrices(Camera& camera)
	{
		std::vector<glm::mat4> ret;
		std::vector<float> shadowCascadeDistances = GetShadowCascadeLevels(camera.farPlane);

		ImGui::Begin("Light space matrices");
		for (size_t i = 0; i < shadowCascadeLevels + 1; ++i)
		{
			if (i == 0)
			{
				ret.push_back(GetLightSpaceMatrix(camera, camera.nearPlane, shadowCascadeDistances[i]));
			}
			else if (i < shadowCascadeLevels)
			{
				ret.push_back(GetLightSpaceMatrix(camera, shadowCascadeDistances[i - 1], shadowCascadeDistances[i]));
			}
			else
			{
				ret.push_back(GetLightSpaceMatrix(camera, shadowCascadeDistances[i - 1], camera.farPlane));
			}
		}
		ImGui::End();
		return ret;
	}

	void DirectionalLight::GenerateShadowMap(Shader& shader, Model& model, Camera& camera)
	{
		if (!shadowMapInitialized) {
			InitShadowMap();
		}
		// Get current viewport size to reset after
		GLint m_viewport[4];
		glGetIntegerv(GL_VIEWPORT, m_viewport);

		shader.use();
		shader.setMat4("lightSpaceMatrix", GetLightSpaceMatrix(camera, camera.nearPlane, camera.farPlane));

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

	void DirectionalLight::GenerateCascadedShadowMap(Model& model, Camera& camera)
	{
		if (!shadowMapInitialized) {
			InitCascadedShadowMaps();
		}

		// Get current viewport size to reset after
		GLint m_viewport[4];
		glGetIntegerv(GL_VIEWPORT, m_viewport);

		cascadedShader.use();
		std::vector<glm::mat4> lightSpaceMatrices = GetLightSpaceMatrices(camera);
		glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
		for (size_t i = 0; i < lightSpaceMatrices.size(); ++i)
		{
			glBufferSubData(GL_UNIFORM_BUFFER, i * sizeof(glm::mat4x4), sizeof(glm::mat4x4), &lightSpaceMatrices[i]);
		}
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_TEXTURE_2D_ARRAY, depthMap, 0);
		glClear(GL_DEPTH_BUFFER_BIT);
		//glActiveTexture(GL_TEXTURE0);
		//shader.setMat4("model", model.transform);
		model.Draw(cascadedShader);

		// Reset framebuffer, viewport
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	std::vector<float> DirectionalLight::GetShadowCascadeLevels(float cameraFarPlane)
	{
		return std::vector<float> { cameraFarPlane / 50.0f, cameraFarPlane / 25.0f, cameraFarPlane / 10.0f, cameraFarPlane / 2.0f };
	}

	void DirectionalLight::InitUBO()
	{
		glGenBuffers(1, &matricesUBO);
		glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4x4) * 16, nullptr, GL_STATIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, matricesUBO);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void DirectionalLight::InitBuffers()
	{
		std::vector<float> vertices = Cube::GetUnindexedVertices();

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);
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

	void DirectionalLight::InitCascadedShadowMaps()
	{
		glGenFramebuffers(1, &depthMapFBO);
		// create depth texture
		glGenTextures(1, &depthMap);
		glBindTexture(GL_TEXTURE_2D_ARRAY, depthMap);
		glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32F, SHADOW_WIDTH, SHADOW_HEIGHT, shadowCascadeLevels + 1, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		constexpr float bordercolor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, bordercolor);


		// attach depth texture as FBO's depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!";
			throw 0;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		shadowMapInitialized = true;
	}

	*/
}