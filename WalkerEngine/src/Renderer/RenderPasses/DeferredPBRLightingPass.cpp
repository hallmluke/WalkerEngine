#include "walkerpch.h"
#include "DeferredPBRLightingPass.h"

namespace Walker {

	DeferredPBRLightingPass::DeferredPBRLightingPass(uint32_t width, uint32_t height)
	{
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

	void DeferredPBRLightingPass::Draw() const
	{

	}

	RenderPassOutput DeferredPBRLightingPass::GetOutput(std::string name) const
	{
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

	void DeferredPBRLightingPass::DrawScene(Scene& scene) const
	{
		// TODO: Automatically bind correct framebuffer based on render graph
		RenderCommand::BindDefaultFramebuffer();
		m_Shader->Bind();

		for (auto input : m_Inputs) {
			m_Shader->SetInt(input.Name, input.Slot);
		}

		m_Shader->SetVec3("camPos", scene.GetCamera()->GetPosition());

		BindInputs();

		// Set lighting
		

		m_Quad.Draw();
	}

	void DeferredPBRLightingPass::BindTextures()
	{

	}
}