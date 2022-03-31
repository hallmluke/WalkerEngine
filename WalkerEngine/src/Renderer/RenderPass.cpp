#include "walkerpch.h"
#include "RenderPass.h"

namespace Walker {

	void RenderPass::BindInputs() const
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

	std::vector<RenderPassInput> RenderPass::GetInputs() const
	{
		return m_Inputs;
	}

	RenderPassInput RenderPass::GetInput(std::string name) const
	{
		for (auto input : m_Inputs) {
			if (input.Name == name) {
				return input;
			}
		}

		W_CORE_ERROR("No input found with name '{0}'", name);

		return RenderPassInput();
	}

	RenderPassOutput RenderPass::GetOutput(std::string name) const
	{
		for (auto output : m_Outputs) {
			if (output.Name == name) {
				return output;
			}
		}

		W_CORE_ERROR("No output found for with name '{0}'", name);

		return RenderPassOutput();
	}

	void RenderPass::LinkToInput(std::string inputName, RenderPassOutput output)
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
			W_CORE_ERROR("No input found with name {0} in DepthOfFieldPass", inputName);
		}
	}

	void RenderPass::OnImGuiRender()
	{
	}

}