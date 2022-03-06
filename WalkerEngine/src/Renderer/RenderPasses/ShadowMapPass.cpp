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
		std::vector<glm::vec3> positions;
		std::vector<std::shared_ptr<PointLight>> lights = scene.GetPointLights(positions);

		// TODO: Implement max per frame with caching?
		for (int i = 0; i < lights.size(); i++) {
			lights[i]->GenerateShadowMap(scene, positions[i]);
		}
	}
	std::shared_ptr<Framebuffer> ShadowMapPass::GetFramebuffer() const
	{
		return nullptr;
	}
	uint32_t ShadowMapPass::GetFinalOutputRendererId() const
	{
		// No final output
		return uint32_t();
	}
	void ShadowMapPass::Resize(uint32_t width, uint32_t height)
	{
		// Resizing view shouldn't affect shadow maps
	}
}