#include "walkerpch.h"
#include "PointLight.h"

#include "Model.h"
#include "Renderer/Shader.h"
#include "imgui/imgui.h"
#include "Scene.h"


namespace Walker {

	PointLight::PointLight()
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

	}

	void PointLight::GenerateShadowMap(Scene& scene, glm::vec3 position)
	{
		m_ShadowMapFramebuffer->Bind();
		m_ShadowMapShader->Bind();

		UpdateShadowTransforms(position);

		for (uint32_t i = 0; i < m_ShadowTransforms.size(); i++) {
			m_ShadowMapShader->SetMat4("shadowMatrices[" + std::to_string(i) + "]", m_ShadowTransforms[i]);
		}
		m_ShadowMapShader->SetFloat("far_plane", m_ShadowMapFarPlane);
		m_ShadowMapShader->SetVec3("lightPos", position);
		RenderCommand::Clear();
		//model.Draw(m_ShadowMapShader);
		scene.DrawMeshes(m_ShadowMapShader);
	}

	void PointLight::BindShadowMap(uint32_t slot)
	{
		m_ShadowMapFramebuffer->BindDepthAttachment(slot);
	}

	void PointLight::UpdateShadowTransforms(glm::vec3 position)
	{
		std::vector<glm::mat4> shadowTransforms;

		shadowTransforms.push_back(m_ShadowMapProjection *
			glm::lookAt(position, position + glm::vec3(1.0, 0.0, 0.0)
				, glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(m_ShadowMapProjection *
			glm::lookAt(position, position + glm::vec3(-1.0, 0.0, 0.0)
				, glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(m_ShadowMapProjection *
			glm::lookAt(position, position + glm::vec3(0.0, 1.0, 0.0)
				, glm::vec3(0.0, 0.0, 1.0)));
		shadowTransforms.push_back(m_ShadowMapProjection *
			glm::lookAt(position, position + glm::vec3(0.0, -1.0, 0.0)
				, glm::vec3(0.0, 0.0, -1.0)));
		shadowTransforms.push_back(m_ShadowMapProjection *
			glm::lookAt(position, position + glm::vec3(0.0, 0.0, 1.0)
				, glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(m_ShadowMapProjection *
			glm::lookAt(position, position + glm::vec3(0.0, 0.0, -1.0)
				, glm::vec3(0.0, -1.0, 0.0)));

		m_ShadowTransforms = shadowTransforms;
	}

}