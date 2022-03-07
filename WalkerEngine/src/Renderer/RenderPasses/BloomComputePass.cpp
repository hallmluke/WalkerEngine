#include "walkerpch.h"
#include "BloomComputePass.h"

namespace Walker {

	BloomComputePass::BloomComputePass(uint32_t width, uint32_t height)
	{
		TextureSpecification spec;
		spec.Width = width / 2;
		spec.Height = height / 2;
		spec.MagFilter = TextureFilterType::LINEAR;
		spec.MinFilter = TextureFilterType::LINEAR;
		spec.Type = TextureType::FLOAT;
		spec.TextureFormat = TextureFormat::RGBA16F;
		spec.WrapS = TextureWrapType::CLAMP_EDGE;
		spec.WrapT = TextureWrapType::CLAMP_EDGE;

		m_Half = Texture2D::Create(spec);
		m_Quarter = Texture2D::Create(width / 4, height / 4);
		m_Eigth = Texture2D::Create(width / 8, height / 8);

		m_Prefilter = ComputeShader::Create("BloomPrefilter", "Shaders/bloomprefilter.comp");

		m_Inputs = { { "img_input", 0, this } };
		//m_Downsample = ComputeShader::Create("Downsample", "Shaders/downsample.comp");
		//m_Upsample = ComputeShader::Create("Upsample", "Shaders/upsample.comp");
	}

	void BloomComputePass::BindInputs() const
	{
		for (auto input : m_Inputs) {

			auto it = m_Links.find(input.Name);

			if (it != m_Links.end()) {
				RenderPass* outPass = it->second.RenderPass;
				outPass->BindOutputImage(it->second.Slot, input.Slot);
			}
			else {
				W_CORE_ERROR("No output linked for input {0}", input.Name);
			}
		}
	}

	void BloomComputePass::BindOutput(uint32_t outputSlot, uint32_t inputSlot) const
	{
		m_Half->Bind(inputSlot);
	}

	void BloomComputePass::BindOutputImage(uint32_t outputSlot, uint32_t inputSlot) const
	{
		m_Half->BindImage(inputSlot);
	}

	void BloomComputePass::DrawScene(Scene& scene) const
	{
		m_Prefilter->Bind();
		BindInputs();
		m_Half->BindImage(1);
		m_Prefilter->Dispatch(m_Half->GetWidth(), m_Half->GetHeight(), 1);
		m_Prefilter->Barrier();
	}
	std::shared_ptr<Framebuffer> BloomComputePass::GetFramebuffer() const
	{
		return std::shared_ptr<Framebuffer>();
	}

	uint32_t BloomComputePass::GetFinalOutputRendererId() const
	{
		return m_Half->GetRendererID();
	}

	void BloomComputePass::Resize(uint32_t width, uint32_t height)
	{

	}
}