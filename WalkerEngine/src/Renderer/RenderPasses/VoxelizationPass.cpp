#include "walkerpch.h"
#include "VoxelizationPass.h"
#include "Renderer/RenderCommand.h"

namespace Walker {

	VoxelizationPass::VoxelizationPass()
	{
		TextureSpecification spec;
		spec.Width = 128;
		spec.Height = 128;
		spec.Depth = 128;
		spec.MagFilter = TextureFilterType::NEAREST;
		spec.MinFilter = TextureFilterType::NEAREST;
		spec.Type = TextureType::FLOAT;
		spec.TextureFormat = TextureFormat::RGBA16F;
		spec.WrapS = TextureWrapType::CLAMP_EDGE;
		spec.WrapT = TextureWrapType::CLAMP_EDGE;
		spec.WrapR = TextureWrapType::CLAMP_EDGE;

		m_VoxelTex = Texture3D::Create(spec);
		m_VoxelizationShader = Shader::Create("Voxelization", "Shaders/voxelize.vert", "Shaders/voxelize.frag", "Shaders/voxelize.geom");

		m_Volume = std::make_shared<Volume>(m_VoxelTex);
		m_VisualizationShader = Shader::Create("VoxelVisualization", "Shaders/voxel_visualization.vert", "Shaders/voxel_visualization.frag");

		m_VolumeCompShader = ComputeShader::Create("VolumeTest", "Shaders/volume_test.comp");

		m_Cache = false;
	}
	void VoxelizationPass::BindInputs() const
	{
		// No inputs
	}
	void VoxelizationPass::BindOutput(uint32_t outputSlot, uint32_t inputSlot) const
	{
		m_VoxelTex->Bind(inputSlot);
	}
	void VoxelizationPass::BindOutputImage(uint32_t outputSlot, uint32_t inputSlot) const
	{
		m_VoxelTex->BindImage(inputSlot);
	}

	void VoxelizationPass::DrawScene(Scene& scene) const
	{
		if (!m_Cache) {
			RenderCommand::DisableDepthTest();
			RenderCommand::DisableBackfaceCulling();
			//RenderCommand::SetViewport(0, 0, 128, 128);
			m_VoxelizationShader->Bind();
			m_VoxelTex->BindImage();
			scene.Voxelize(m_VoxelizationShader);
			m_VoxelTex->GenerateMipMaps();
			RenderCommand::EnableDepthTest();
			m_Cache = true;
		}

		/*m_VoxelTex->BindImage(0);
		m_VolumeCompShader->Bind();
		m_VolumeCompShader->Dispatch(128 / 8, 128 / 8, 128 / 8);
		m_VolumeCompShader->Barrier();*/

		//RenderCommand::EnableBackfaceCulling();
		RenderCommand::Clear();
		RenderCommand::EnableDepthTest();
		m_Volume->Draw(m_VisualizationShader, scene.GetCamera()->GetViewMatrix(), scene.GetCamera()->GetProjectionMatrix(), scene.GetCamera()->GetPosition());
	}

	std::shared_ptr<Framebuffer> VoxelizationPass::GetFramebuffer() const
	{
		return nullptr;
	}
	uint32_t VoxelizationPass::GetFinalOutputRendererId() const
	{
		return m_VoxelTex->GetRendererID();
	}

	void VoxelizationPass::Resize(uint32_t width, uint32_t height)
	{
		// Not applicable
	}
}