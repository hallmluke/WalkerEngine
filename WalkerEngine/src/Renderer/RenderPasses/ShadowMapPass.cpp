#include "walkerpch.h"
#include "ShadowMapPass.h"

namespace Walker {
	ShadowMapPass::ShadowMapPass()
	{
	}
	void ShadowMapPass::BindInputs() const
	{
	}
	void ShadowMapPass::BindOutput(uint32_t outputSlot, uint32_t inputSlot) const
	{
	}
	void ShadowMapPass::Draw() const
	{
	}
	RenderPassInput ShadowMapPass::GetInput(std::string name) const
	{
		return RenderPassInput();
	}
	RenderPassOutput ShadowMapPass::GetOutput(std::string name) const
	{
		return RenderPassOutput();
	}
	void ShadowMapPass::LinkToInput(std::string inputName, RenderPassOutput output)
	{
	}

	void ShadowMapPass::DrawScene(Scene& scene) const
	{
		scene.GetDirectionalLight()->GenerateCascadedShadowMap(scene);
		std::vector<std::shared_ptr<PointLight>> lights = scene.GetPointLights();

		// TODO: Implement max per frame with caching?
		for (auto light : lights) {
			light->GenerateShadowMap(scene);
		}
	}
	uint32_t ShadowMapPass::GetFinalOutputRendererId() const
	{
		// No final output
		return uint32_t();
	}
}