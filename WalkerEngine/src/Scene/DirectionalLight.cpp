#include "walkerpch.h"
#include "DirectionalLight.h"
#include "Renderer/Shader.h"
#include "imgui\imgui.h";
#include <glm/gtx/string_cast.hpp>
#include "Camera.h"
#include "Model.h"

namespace Walker {
	
	DirectionalLight::DirectionalLight(glm::vec3 dir, bool shadowMapEnabled) : m_Direction(dir)
	{
		m_Name = "Directional Light";
		m_AmbientIntensity = 0.1f;
		m_DiffuseIntensity = 3.0f;
		m_SpecularIntensity = 1.0f;
		m_Color = glm::vec3(0.8f);

		m_ShadowMapEnabled = shadowMapEnabled;

		FramebufferSpecification fbSpec;
		fbSpec.Attachments = {
			{ "Depth", FramebufferTextureFormat::DEPTH32F, FramebufferTextureType::FLOAT, FramebufferTextureTarget::TEXTURE_2D_ARRAY }
		}; // TODO: Border for texture
		fbSpec.Width = m_ShadowMapWidth;
		fbSpec.Height = m_ShadowMapHeight;
		fbSpec.Depth = 5; // TODO: Correspond to number of cascades
		fbSpec.Samples = 1;
		m_ShadowMapFramebuffer = Framebuffer::Create(fbSpec);

		m_LightMatricesUniformBuffer = UniformBuffer::Create(sizeof(glm::mat4) * 16, 0);

		SetShadowCascadeLevels(70.0f); // TODO: retrieve from camera
		m_ShadowMapShader = Shader::Create("Depth", "Shaders/depth_shader_cascades.vert", "Shaders/depth_shader.frag", "Shaders/depth_shader_cascades.geom"); // TODO: Use shader library
	}

	glm::mat4 DirectionalLight::GetLightSpaceMatrix(Camera& camera, const float nearPlane, const float farPlane)
	{
		std::vector<glm::vec4> corners = camera.GetFrustumCornersWorldSpace(glm::perspective(glm::radians(camera.GetZoom()), camera.GetAspectRatio(), nearPlane, farPlane), camera.GetViewMatrix());

		glm::vec3 center = glm::vec3(0, 0, 0);
		for (const auto& v : corners)
		{
			center += glm::vec3(v);
		}
		center /= corners.size();

		glm::vec3 centerDirection = center - m_Direction;


		const auto lightView = glm::lookAt(
			center - m_Direction,
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
				maxZ = 0.5f;
			}
			//maxZ += 0.1;
			maxZ *= debugDistance;
		}

		const glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);

		return lightProjection * lightView;
	}

	std::vector<glm::mat4> DirectionalLight::GetLightSpaceMatrices(Camera& camera)
	{
		std::vector<glm::mat4> matrices;

		for (size_t i = 0; i < m_ShadowCascadeLevels.size() + 1; ++i)
		{
			if (i == 0)
			{
				matrices.push_back(GetLightSpaceMatrix(camera, camera.GetNearPlane(), m_ShadowCascadeLevels[i]));
			}
			else if (i < m_ShadowCascadeLevels.size())
			{
				matrices.push_back(GetLightSpaceMatrix(camera, m_ShadowCascadeLevels[i - 1], m_ShadowCascadeLevels[i]));
			}
			else
			{
				matrices.push_back(GetLightSpaceMatrix(camera, m_ShadowCascadeLevels[i - 1], camera.GetFarPlane()));
			}
		}
		return matrices;
	}

	void DirectionalLight::GenerateCascadedShadowMap(Model& model, Camera& camera)
	{
		// Upload light space matrices to uniform buffer
		std::vector<glm::mat4> lightSpaceMatrices = GetLightSpaceMatrices(camera);
		for (size_t i = 0; i < lightSpaceMatrices.size(); ++i)
		{
			m_LightMatricesUniformBuffer->SetData(&lightSpaceMatrices[i], sizeof(glm::mat4x4), i * sizeof(glm::mat4x4));
		}

		m_ShadowMapFramebuffer->Bind();
		m_ShadowMapShader->Bind();
		RenderCommand::Clear();
		model.Draw(m_ShadowMapShader);
	}

	void DirectionalLight::BindShadowMap(uint32_t slot) const
	{
		m_ShadowMapFramebuffer->BindDepthAttachment(slot);
	}

	void DirectionalLight::SetShadowCascadeLevels(float cameraFarPlane)
	{
		// TODO: Dynamic number of cascades
		m_ShadowCascadeLevels = std::vector<float> { cameraFarPlane / 50.0f, cameraFarPlane / 25.0f, cameraFarPlane / 10.0f, cameraFarPlane / 2.0f };
	}

	/*
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
	}*/

}