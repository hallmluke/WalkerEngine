#include "walkerpch.h"
#include "PointLight.h"

#include "Model.h"
#include "Renderer/Shader.h"
#include "imgui/imgui.h"
#include "Scene.h"


namespace Walker {

	PointLight::PointLight(glm::vec3 pos) : m_Position(pos)
	{
		m_Name = "Point Light";
		m_AmbientIntensity = 0.1f;
		m_DiffuseIntensity = 0.8f;
		m_SpecularIntensity = 1.0f;

		m_ConstantAttenuation = 1.0f;
		m_LinearAttenuation = 0.09f;
		m_QuadraticAttenuation = 0.032f;

		FramebufferSpecification fbSpec;
		fbSpec.Attachments = {
			{ "Depth", FramebufferTextureFormat::DEPTH32F, FramebufferTextureType::FLOAT, FramebufferTextureTarget::TEXTURE_CUBE_MAP }
		};
		fbSpec.Width = m_ShadowMapWidth;
		fbSpec.Height = m_ShadowMapHeight;
		fbSpec.Depth = 1;
		fbSpec.Samples = 1;
		m_ShadowMapFramebuffer = Framebuffer::Create(fbSpec);

		m_ShadowMapShader = Shader::Create("DepthCubeShader", "Shaders/depth_shader_cube.vert", "Shaders/depth_shader_cube.frag", "Shaders/depth_shader_cube.geom");

		UpdateShadowTransforms();
	}

	void PointLight::GenerateShadowMap(Scene& scene)
	{
		m_ShadowMapFramebuffer->Bind();
		m_ShadowMapShader->Bind();

		for (uint32_t i = 0; i < m_ShadowTransforms.size(); i++) {
			m_ShadowMapShader->SetMat4("shadowMatrices[" + std::to_string(i) + "]", m_ShadowTransforms[i]);
		}
		m_ShadowMapShader->SetFloat("far_plane", m_ShadowMapFarPlane);
		m_ShadowMapShader->SetVec3("lightPos", m_Position);
		RenderCommand::Clear();
		//model.Draw(m_ShadowMapShader);
		scene.DrawMeshes(m_ShadowMapShader);
	}

	void PointLight::BindShadowMap(uint32_t slot)
	{
		m_ShadowMapFramebuffer->BindDepthAttachment(slot);
	}

	void PointLight::UpdateShadowTransforms()
	{
		std::vector<glm::mat4> shadowTransforms;

		shadowTransforms.push_back(m_ShadowMapProjection *
			glm::lookAt(m_Position, m_Position + glm::vec3(1.0, 0.0, 0.0)
				, glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(m_ShadowMapProjection *
			glm::lookAt(m_Position, m_Position + glm::vec3(-1.0, 0.0, 0.0)
				, glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(m_ShadowMapProjection *
			glm::lookAt(m_Position, m_Position + glm::vec3(0.0, 1.0, 0.0)
				, glm::vec3(0.0, 0.0, 1.0)));
		shadowTransforms.push_back(m_ShadowMapProjection *
			glm::lookAt(m_Position, m_Position + glm::vec3(0.0, -1.0, 0.0)
				, glm::vec3(0.0, 0.0, -1.0)));
		shadowTransforms.push_back(m_ShadowMapProjection *
			glm::lookAt(m_Position, m_Position + glm::vec3(0.0, 0.0, 1.0)
				, glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(m_ShadowMapProjection *
			glm::lookAt(m_Position, m_Position + glm::vec3(0.0, 0.0, -1.0)
				, glm::vec3(0.0, -1.0, 0.0)));

		m_ShadowTransforms = shadowTransforms;
	}

	void PointLight::ControlWindow()
	{
		if (ImGui::Begin(m_Name.c_str())) {
			ImGui::Text("Position");
			if (ImGui::SliderFloat("X", &m_Position.x, -20.0f, 20.0f)) {
				UpdateShadowTransforms();
			}
			if (ImGui::SliderFloat("Y", &m_Position.y, -20.0f, 20.0f)) {
				UpdateShadowTransforms();
			}
			if (ImGui::SliderFloat("Z", &m_Position.z, -20.0f, 20.0f)) {
				UpdateShadowTransforms();
			}

			ImGui::Text("Intensity/Color");
			ImGui::SliderFloat("Ambient Intensity", &m_AmbientIntensity, 0.01f, 0.5f);
			ImGui::SliderFloat("Diffuse Intensity", &m_DiffuseIntensity, 0.4f, 1.0f);
			ImGui::SliderFloat("Specular Intensity", &m_SpecularIntensity, 0.8f, 1.2f);

			ImGui::Text("Falloff");
			ImGui::SliderFloat("Constant", &m_ConstantAttenuation, 0.05f, 10.0f);
			ImGui::SliderFloat("Linear", &m_LinearAttenuation, 0.0001f, 4.0f);
			ImGui::SliderFloat("Quadratic", &m_QuadraticAttenuation, 0.0000001f, 0.35f);

			//ImGui::Text("Shadow Map");
			//ImGui::SliderFloat("Bias", &bias, 0.001f, 0.1f);
			//
			//ImGui::Text("Debug");
			//ImGui::Checkbox("Draw", &drawDebugEnabled);
			//ImGui::SliderFloat("Light scale", &debugDrawSize, 0.01f, 3.0f);
			//ImGui::Checkbox("Debug Shadow", &debugShadow);
		}
		ImGui::End();
	}

}