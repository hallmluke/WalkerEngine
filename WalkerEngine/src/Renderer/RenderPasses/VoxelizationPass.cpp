#include "walkerpch.h"
#include "VoxelizationPass.h"
#include "Renderer/RenderCommand.h"
#include "Platform/OpenGL/OpenGLUtils.h"

#include <imgui.h>

namespace Walker {

	VoxelizationPass::VoxelizationPass()
	{
		/*TextureSpecification spec;
		spec.Width = 128;
		spec.Height = 128;
		spec.Depth = 128;
		spec.MagFilter = TextureFilterType::LINEAR;
		spec.MinFilter = TextureFilterType::LINEAR;
		spec.Type = TextureType::FLOAT;
		spec.TextureFormat = TextureFormat::RGBA16F;
		spec.WrapS = TextureWrapType::CLAMP_EDGE;
		spec.WrapT = TextureWrapType::CLAMP_EDGE;
		spec.WrapR = TextureWrapType::CLAMP_EDGE;

		m_VoxelTex = Texture3D::Create(spec);*/
		m_VoxelizationShader = Shader::Create("Voxelization", "Shaders/voxelize.vert", "Shaders/voxelize.frag", "Shaders/voxelize.geom");

		//m_Volume = std::make_shared<Volume>(m_VoxelTex);
		//m_VisualizationShader = Shader::Create("VoxelVisualization", "Shaders/voxel_visualization.vert", "Shaders/voxel_visualization.frag");

		//m_VolumeCompShader = ComputeShader::Create("VolumeTest", "Shaders/volume_test.comp");

		m_VoxelBufferToTexShader = ComputeShader::Create("VoxelBufferToTex", "Shaders/voxel_buffer_to_texture.comp");

		m_VoxelBuffer = ShaderStorageBuffer::Create(sizeof(VoxelType) * 128 * 128 * 128, 1);
		m_Cache = false;

		m_Outputs = { {"VoxelTex", 0, this} };
	}
	void VoxelizationPass::BindInputs() const
	{
		// No inputs
	}
	void VoxelizationPass::BindOutput(uint32_t outputSlot, uint32_t inputSlot) const
	{
		//m_VoxelTex->Bind(inputSlot);
	}
	void VoxelizationPass::BindOutputImage(uint32_t outputSlot, uint32_t inputSlot) const
	{
		//m_VoxelTex->BindImage(inputSlot);
	}

	void VoxelizationPass::DrawScene(Scene& scene) const
	{
		RenderCommand::BindDefaultFramebuffer();
		RenderCommand::SetViewport(0, 0, 1600, 900);

		
		if (!m_Cache) {
			

			std::vector<glm::vec3> positions;
			std::vector<glm::vec3> scales;
			auto probes = scene.GetGIProbes(positions, scales);

			//for (auto probe : probes) {
			for (size_t i = 0; i < probes.size(); i++) {

				auto probe = probes[i];

				if (!probe->VoxelTex) {
					TextureSpecification spec;
					spec.Width = probe->Subdiv;
					spec.Height = probe->Subdiv;
					spec.Depth = probe->Subdiv;
					spec.MagFilter = TextureFilterType::LINEAR;
					spec.MinFilter = TextureFilterType::LINEAR_MIPMAP_LINEAR;
					spec.Type = TextureType::FLOAT;
					spec.TextureFormat = TextureFormat::RGBA16;
					spec.WrapS = TextureWrapType::CLAMP_EDGE;
					spec.WrapT = TextureWrapType::CLAMP_EDGE;
					spec.WrapR = TextureWrapType::CLAMP_EDGE;

					probe->VoxelTex = Texture3D::Create(spec);
				}

				m_VoxelBuffer = ShaderStorageBuffer::Create(sizeof(VoxelType) * probe->Subdiv * probe->Subdiv * probe->Subdiv, 1);
				
				glm::mat4 lightSpaceMatrix = glm::mat4(1.0f);
				auto dirLight = scene.GetDirectionalLight();
				if (dirLight) {
					lightSpaceMatrix = dirLight->GetVoxelLightSpaceMatrix(scene);
					
					dirLight->GenerateVoxelShadowMap(scene, lightSpaceMatrix);
					dirLight->BindVoxelShadowMap(5);
					RenderCommand::BindDefaultFramebuffer();
					RenderCommand::SetViewport(0, 0, 1600, 900);
					OpenGLUtils::LogErrors();

				}

				RenderCommand::DisableDepthTest();
				RenderCommand::DisableBackfaceCulling();

				//RenderCommand::SetViewport(0, 0, 128, 128);
				m_VoxelizationShader->Bind();
				//m_VoxelTex->BindImage(1);
				probe->VoxelTex->BindImage(1);
				m_VoxelizationShader->SetVec3("GIPosition", positions[i]);
				m_VoxelizationShader->SetVec3("GIScale", scales[i]);
				m_VoxelizationShader->SetInt("GISubdiv", probe->Subdiv);
				
				SetPointLightShaderUniforms(scene);
				SetDirectionalLightShaderUniforms(scene);
				m_VoxelizationShader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);

				glm::mat4 voxelProjection = glm::ortho(-scales[i].x, scales[i].x, -scales[i].y, scales[i].y, -scales[i].z, scales[i].z);
				glm::mat4 voxelView = glm::lookAt(glm::vec3(0.0f, 0.0f, scales[i].z / 2), positions[i], glm::vec3(0.0f, 1.0f, 0.0f));

				scene.Voxelize(m_VoxelizationShader, voxelProjection, voxelView);

				m_VoxelBufferToTexShader->Bind();

				m_VoxelBufferToTexShader->Dispatch(probe->Subdiv / 8, probe->Subdiv / 8, probe->Subdiv / 8);
				m_VoxelBufferToTexShader->Barrier();
				//m_VoxelTex->GenerateMipMaps();
				probe->VoxelTex->GenerateMipMaps();
				RenderCommand::EnableDepthTest();
			}
			m_Cache = true;
		}

		/*m_VoxelTex->BindImage(0);
		m_VolumeCompShader->Bind();
		m_VolumeCompShader->Dispatch(128 / 8, 128 / 8, 128 / 8);
		m_VolumeCompShader->Barrier();*/

		//RenderCommand::EnableBackfaceCulling();
		RenderCommand::Clear();
		RenderCommand::EnableDepthTest();
		//RenderCommand::SetViewport(0, 0, 1600, 900);
		//m_Volume->Draw(m_VisualizationShader, scene.GetCamera()->GetViewMatrix(), scene.GetCamera()->GetProjectionMatrix(), scene.GetCamera()->GetPosition());
	}

	std::shared_ptr<Framebuffer> VoxelizationPass::GetFramebuffer() const
	{
		return nullptr;
	}
	uint32_t VoxelizationPass::GetFinalOutputRendererId() const
	{
		return 0;
		//return m_VoxelTex->GetRendererID();
	}

	void VoxelizationPass::Resize(uint32_t width, uint32_t height)
	{
		// Not applicable
	}

	void VoxelizationPass::OnImGuiRender()
	{
		ImGui::Begin("Voxelization");
		ImGui::Checkbox("Cache", &m_Cache);
		ImGui::End();
	}

	void VoxelizationPass::SetDirectionalLightShaderUniforms(Scene& scene) const
	{
		std::shared_ptr<DirectionalLight> light = scene.GetDirectionalLight();

		m_VoxelizationShader->SetVec3("dirLight.direction", light->GetDirection());
		m_VoxelizationShader->SetVec3("dirLight.color", light->GetColor());
		m_VoxelizationShader->SetVec3("dirLight.ambient", glm::vec3(light->GetAmbientIntensity()));
		m_VoxelizationShader->SetVec3("dirLight.diffuse", glm::vec3(light->GetDiffuseIntensity()));
		m_VoxelizationShader->SetVec3("dirLight.specular", glm::vec3(light->GetSpecularIntensity()));
		m_VoxelizationShader->SetFloat("dirLight.shadowBias", 0.005f);
		m_VoxelizationShader->SetInt("dirLight.shadowMap", 5);
	}

	void VoxelizationPass::SetPointLightShaderUniforms(Scene& scene) const
	{
		size_t maxLights = 8;
		std::vector<glm::vec3> positions;
		auto lights = scene.GetPointLights(positions);

		uint32_t maxedLights = std::min(maxLights, lights.size());

		for (int i = 0; i < maxLights; i++) {
			std::string lightPrefix = "lights[" + std::to_string(i) + "]";
			int pointLightStartSlot = 6;
			if (i < maxedLights) {
				auto light = lights[i];
				auto position = positions[i];
				m_VoxelizationShader->SetVec3(lightPrefix + ".position", position);
				m_VoxelizationShader->SetVec3(lightPrefix + ".ambient", glm::vec3(light->GetAmbientIntensity()));
				m_VoxelizationShader->SetVec3(lightPrefix + ".diffuse", glm::vec3(light->GetDiffuseIntensity()));
				m_VoxelizationShader->SetVec3(lightPrefix + ".specular", glm::vec3(light->GetSpecularIntensity()));
				m_VoxelizationShader->SetFloat(lightPrefix + ".constant", light->GetConstantAttenuation());
				m_VoxelizationShader->SetFloat(lightPrefix + ".linear", light->GetLinearAttenuation());
				m_VoxelizationShader->SetFloat(lightPrefix + ".quadratic", light->GetQuadraticAttenuation());


				light->BindShadowMap(pointLightStartSlot + i);
				m_VoxelizationShader->SetInt(lightPrefix + ".depthMap", pointLightStartSlot + i);
				m_VoxelizationShader->SetFloat(lightPrefix + ".far_plane", light->GetShadowMapFarPlane());
				m_VoxelizationShader->SetFloat(lightPrefix + ".bias", 0.05f);
			}
			// TODO: This is done to avoid issues with a samplerCube bound to a 2d texture, figure out a better way to do this
			else if (lights.size() > 0) {
				//auto light = lights[lights.size() - 1];
				//light->BindShadowMap(pointLightStartSlot + i);
				m_VoxelizationShader->SetInt(lightPrefix + ".depthMap", pointLightStartSlot + lights.size() - 1);
				//m_VoxelizationShader->SetInt(lightPrefix + ".depthMap", 0);
			}
			else {
				m_VoxelizationShader->SetInt(lightPrefix + ".depthMap", pointLightStartSlot);
			}
		}

		m_VoxelizationShader->SetInt("numberOfLights", maxedLights);
	}
}