#include "walkerpch.h"
#include "DeferredPBRLightingPass.h"
#include "Renderer/RenderCommand.h"

#include <imgui.h>

namespace Walker {

	DeferredPBRLightingPass::DeferredPBRLightingPass(uint32_t width, uint32_t height)
	{
		FramebufferSpecification fbSpec;
		fbSpec.Attachments = {
			{ "gColor", FramebufferTextureFormat::RGBA16F, FramebufferTextureType::UNSIGNED_BYTE, FramebufferTextureTarget::TEXTURE_2D }
		};

		fbSpec.Width = width;
		fbSpec.Height = height;
		fbSpec.Samples = 1;
		m_Framebuffer = Framebuffer::Create(fbSpec);
		m_Shader = Shader::Create("Deferred PBR Lighting Pass", "Shaders/deferred_shading.vert", "Shaders/pbr_deferred.frag");
	}

	void DeferredPBRLightingPass::BindInputs() const
	{
		for (auto input : m_Inputs) {

			auto it = m_Links.find(input.Name);

			if (it != m_Links.end()) {
				RenderPass* outPass = it->second.RenderPass;
				outPass->BindOutput(it->second.Slot, input.Slot);
			}
			else {
				W_CORE_ERROR("No output linked for input {0}", input.Name);
			}
		}
	}

	void DeferredPBRLightingPass::BindOutput(uint32_t outputSlot, uint32_t inputSlot) const
	{
		m_Framebuffer->BindColorAttachment(outputSlot, inputSlot);
	}

	void DeferredPBRLightingPass::BindOutputImage(uint32_t outputSlot, uint32_t inputSlot) const
	{
		m_Framebuffer->BindColorAttachmentImage(outputSlot, inputSlot);
	}

	RenderPassInput DeferredPBRLightingPass::GetInput(std::string name) const
	{
		for (auto input : m_Inputs) {
			if (input.Name == name) {
				return input;
			}
		}

		W_CORE_ERROR("No input found with name '{0}'", name);

		return RenderPassInput();
	}

	RenderPassOutput DeferredPBRLightingPass::GetOutput(std::string name) const
	{
		for (auto output : m_Outputs) {
			if (output.Name == name) {
				return output;
			}
		}

		W_CORE_ERROR("No output found for with name '{0}'", name);

		return RenderPassOutput();
	}

	void DeferredPBRLightingPass::LinkToInput(std::string inputName, RenderPassOutput output)
	{
		RenderPassInput passInput;
		bool found = false;

		for (auto input : m_Inputs) {
			if (input.Name == inputName) {
				passInput = input;
				found = true;
				break;
			}
		}

		if (found) {
			m_Links[inputName] = output;
		}
		else {
			W_CORE_ERROR("No input found with name {0} in DeferredPBRLightingPass", inputName);
		}
	}

	uint32_t DeferredPBRLightingPass::GetFinalOutputRendererId() const
	{
		return m_Framebuffer->GetColorAttachmentRendererID();
	}

	void DeferredPBRLightingPass::DrawScene(Scene& scene) const
	{
		// TODO: Automatically bind correct framebuffer based on render graph
		//RenderCommand::BindDefaultFramebuffer();
		m_Framebuffer->Bind();
		m_Shader->Bind();

		for (auto input : m_Inputs) {
			m_Shader->SetInt(input.Name, input.Slot);
		}

		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> scales;
		auto probes = scene.GetGIProbes(positions, scales);

		for (size_t i = 0; i < probes.size(); i++) {
			// TODO: This only supports binding one GI Probe
			// should either support multiple or pick best probe to use
			auto probe = probes[i];
			probe->VoxelTex->Bind(4);

			m_Shader->SetVec3("GIPosition", positions[i]);
			m_Shader->SetVec3("GIScale", scales[i]);
			m_Shader->SetInt("GISubdiv", probe->Subdiv);

		}

		m_Shader->SetVec3("camPos", scene.GetCamera()->GetPosition());
		m_Shader->SetBool("useVoxelConetrace", m_UseVoxelConetrace);
		m_Shader->SetFloat("aperture", m_Aperture);
		m_Shader->SetFloat("maxDistance", m_MaxDistance);
		m_Shader->SetFloat("stepSize", m_StepSize); 
		m_Shader->SetFloat("indirectMultiplier", m_IndirectMultiplier);
		m_Shader->SetInt("numCones", m_NumCones);
		m_Shader->SetFloat("mipModifier", m_MipModifier);

		m_Shader->SetFloat("inverseCascadeFactor", m_InverseCascadeFactor);

		BindInputs();

		// Set lighting
		SetDirectionalLightShaderUniforms(scene);
		SetPointLightShaderUniforms(scene);

		m_Quad.Draw();
	}

	std::shared_ptr<Framebuffer> DeferredPBRLightingPass::GetFramebuffer() const
	{
		return m_Framebuffer;
	}

	void DeferredPBRLightingPass::BindTextures()
	{

	}

	void DeferredPBRLightingPass::Resize(uint32_t width, uint32_t height)
	{
		m_Framebuffer->Resize(width, height);
	}

	void DeferredPBRLightingPass::OnImGuiRender()
	{
		ImGui::Begin("Voxel Cone Tracing Settings");

		ImGui::Checkbox("Use VCT", &m_UseVoxelConetrace);
		ImGui::SliderFloat("Aperture", &m_Aperture, 0.0f, 5.0f);
		ImGui::SliderFloat("Max Distance", &m_MaxDistance, 0.0f, 200.0f);
		ImGui::SliderFloat("Step Size", &m_StepSize, 0.01f, 3.0f);
		ImGui::SliderFloat("Indirect Multiplier", &m_IndirectMultiplier, 0.01f, 10.0f);
		ImGui::SliderInt("Cones", &m_NumCones, 1, 16);
		ImGui::SliderFloat("Mip Modifier", &m_MipModifier, 0.0f, 5.0f);

		ImGui::End();

		ImGui::Begin("Shadow Bias Testing");
		ImGui::SliderFloat("Bias", &m_ShadowBiasTest, 0.0f, 0.2f);
		ImGui::SliderFloat("Inverse Cascade Factor", &m_InverseCascadeFactor, 0.0f, 0.2f);
		ImGui::End();
	}

	void DeferredPBRLightingPass::SetDirectionalLightShaderUniforms(Scene& scene) const
	{
		std::shared_ptr<DirectionalLight> light = scene.GetDirectionalLight();

		m_Shader->SetVec3("dirLight.direction", light->GetDirection());
		m_Shader->SetVec3("dirLight.color", light->GetColor());
		m_Shader->SetVec3("dirLight.ambient", glm::vec3(light->GetAmbientIntensity()));
		m_Shader->SetVec3("dirLight.diffuse", glm::vec3(light->GetDiffuseIntensity()));
		m_Shader->SetVec3("dirLight.specular", glm::vec3(light->GetSpecularIntensity()));
		m_Shader->SetFloat("dirLight.shadowBias", m_ShadowBiasTest);

		std::vector<float> cascadeDistances = light->GetShadowCascadeLevels();
		m_Shader->SetMat4("view", scene.GetCamera()->GetViewMatrix());
		m_Shader->SetFloat("farPlane", scene.GetCamera()->GetFarPlane());
		m_Shader->SetInt("cascadeCount", cascadeDistances.size());
		for (size_t i = 0; i < cascadeDistances.size(); ++i)
		{
			m_Shader->SetFloat("cascadePlaneDistances[" + std::to_string(i) + "]", cascadeDistances[i]);
		}
		light->BindShadowMap(6);
		m_Shader->SetInt("shadowMap", 6);
	}

	void DeferredPBRLightingPass::SetPointLightShaderUniforms(Scene& scene) const
	{
		size_t maxLights = 12;
		std::vector<glm::vec3> positions;
		auto lights = scene.GetPointLights(positions);

		uint32_t maxedLights = std::min(maxLights, lights.size());

		for (int i = 0; i < maxLights; i++) {
			std::string lightPrefix = "lights[" + std::to_string(i) + "]";
			int pointLightStartSlot = 7;
			if (i < maxedLights) {
				auto light = lights[i];
				auto position = positions[i];
				m_Shader->SetVec3(lightPrefix + ".position", position);
				m_Shader->SetVec3(lightPrefix + ".ambient", glm::vec3(light->GetAmbientIntensity()));
				m_Shader->SetVec3(lightPrefix + ".diffuse", glm::vec3(light->GetDiffuseIntensity()));
				m_Shader->SetVec3(lightPrefix + ".specular", glm::vec3(light->GetSpecularIntensity()));
				m_Shader->SetFloat(lightPrefix + ".constant", light->GetConstantAttenuation());
				m_Shader->SetFloat(lightPrefix + ".linear", light->GetLinearAttenuation());
				m_Shader->SetFloat(lightPrefix + ".quadratic", light->GetQuadraticAttenuation());

				
				light->BindShadowMap(pointLightStartSlot + i);
				m_Shader->SetInt(lightPrefix + ".depthMap", pointLightStartSlot + i);
				m_Shader->SetFloat(lightPrefix + ".far_plane", light->GetShadowMapFarPlane());
				m_Shader->SetFloat(lightPrefix + ".bias", 0.05f);
			}
			// TODO: This is done to avoid issues with a samplerCube bound to a 2d texture, figure out a better way to do this
			else if(lights.size() > 0) {
				//auto light = lights[lights.size() - 1];
				//light->BindShadowMap(pointLightStartSlot + i);
				m_Shader->SetInt(lightPrefix + ".depthMap", pointLightStartSlot + lights.size() - 1);
				//m_Shader->SetInt(lightPrefix + ".depthMap", 0);
			}
			else {
				m_Shader->SetInt(lightPrefix + ".depthMap", pointLightStartSlot);
			}
		}

		m_Shader->SetInt("numberOfLights", maxedLights);
	}
}