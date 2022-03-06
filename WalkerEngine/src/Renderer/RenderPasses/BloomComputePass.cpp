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
		//m_Downsample = ComputeShader::Create("Downsample", "Shaders/downsample.comp");
		//m_Upsample = ComputeShader::Create("Upsample", "Shaders/upsample.comp");
	}

	void BloomComputePass::BindOutput(uint32_t outputSlot, uint32_t inputSlot) const
	{
	}

	void BloomComputePass::DrawScene(Scene& scene) const
	{
		m_Half->BindImage(1);
		m_Prefilter->Dispatch(m_Half->GetWidth(), m_Half->GetHeight(), 1);

	}
	std::shared_ptr<Framebuffer> BloomComputePass::GetFramebuffer() const
	{
		return std::shared_ptr<Framebuffer>();
	}
	uint32_t BloomComputePass::GetFinalOutputRendererId() const
	{
		return uint32_t();
	}
	void BloomComputePass::Resize(uint32_t width, uint32_t height)
	{
	}
}