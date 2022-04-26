#include "walkerpch.h"
#include "VoxelVisualizationPass.h"
#include "Renderer/RenderCommand.h"

namespace Walker {
	VoxelVisualizationPass::VoxelVisualizationPass()
	{
		m_VisualizationShader = Shader::Create("VoxelVisualization", "Shaders/voxel_visualization.vert", "Shaders/voxel_visualization.frag");
		m_Volume = std::make_shared<Volume>();
	}
	void VoxelVisualizationPass::BindInputs() const
	{
	}
	void VoxelVisualizationPass::BindOutput(uint32_t outputSlot, uint32_t inputSlot) const
	{
	}
	void VoxelVisualizationPass::BindOutputImage(uint32_t outputSlot, uint32_t inputSlot) const
	{
	}
	void VoxelVisualizationPass::DrawScene(Scene& scene) const
	{
		RenderCommand::BindDefaultFramebuffer();
		RenderCommand::SetViewport(0, 0, 1600, 900);

		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> scales;
		auto probes = scene.GetGIProbes(positions, scales);

		//for (auto probe : probes) {
		for (size_t i = 0; i < probes.size(); i++) {

			auto probe = probes[i];
			if (probe->VoxelTex) {
				m_Volume->SetTexture(probe->VoxelTex);
			}
		}
		
		RenderCommand::Clear();
		RenderCommand::EnableDepthTest();
		//RenderCommand::SetViewport(0, 0, 1600, 900);
		m_Volume->Draw(m_VisualizationShader, scene.GetCamera()->GetViewMatrix(), scene.GetCamera()->GetProjectionMatrix(), scene.GetCamera()->GetPosition());
	}
	std::shared_ptr<Framebuffer> VoxelVisualizationPass::GetFramebuffer() const
	{
		return std::shared_ptr<Framebuffer>();
	}
	uint32_t VoxelVisualizationPass::GetFinalOutputRendererId() const
	{
		return uint32_t();
	}
	void VoxelVisualizationPass::Resize(uint32_t width, uint32_t height)
	{
	}
	void VoxelVisualizationPass::OnImGuiRender()
	{
	}
}