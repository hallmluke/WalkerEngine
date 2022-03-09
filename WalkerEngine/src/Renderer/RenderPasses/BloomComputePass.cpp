#include "walkerpch.h"
#include "BloomComputePass.h"

namespace Walker {

	BloomComputePass::BloomComputePass(uint32_t width, uint32_t height)
	{
		m_Passes = 6;
		TextureSpecification spec;
		spec.Width = width / 2;
		spec.Height = height / 2;
		spec.MagFilter = TextureFilterType::LINEAR;
		spec.MinFilter = TextureFilterType::LINEAR;
		spec.Type = TextureType::FLOAT;
		spec.TextureFormat = TextureFormat::RGBA16F;
		spec.WrapS = TextureWrapType::CLAMP_EDGE;
		spec.WrapT = TextureWrapType::CLAMP_EDGE;

		m_Textures.push_back(Texture2D::Create(spec));
		for (uint32_t i = 1; i < m_Passes; i++) {
			spec.Width = spec.Width / 2;
			spec.Height = spec.Height / 2;
			m_Textures.push_back(Texture2D::Create(spec));
		}
		/*m_Half = Texture2D::Create(spec);

		spec.Width = width / 4;
		spec.Height = height / 4;

		m_Quarter = Texture2D::Create(spec);

		spec.Width = width / 8;
		spec.Height = height / 8;

		m_Eigth = Texture2D::Create(spec);*/

		m_Prefilter = ComputeShader::Create("BloomPrefilter", "Shaders/bloomprefilter.comp");

		m_Inputs = { { "img_input", 0, this } };
		m_DownsampleHorizontal = ComputeShader::Create("DownsampleHorizontal", "Shaders/gaussian_downsample_horizontal.comp");
		m_DownsampleVertical = ComputeShader::Create("DownsampleVertical", "Shaders/gaussian_blur_vertical.comp");
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
		m_Textures[0]->Bind(outputSlot);
		//m_Half->Bind(inputSlot);
		//m_Quarter->Bind(inputSlot);
		//m_Eigth->Bind(inputSlot);
	}

	void BloomComputePass::BindOutputImage(uint32_t outputSlot, uint32_t inputSlot) const
	{
		m_Textures[0]->Bind(outputSlot);
		//m_Half->BindImage(inputSlot);
		//m_Eigth->BindImage(inputSlot);
	}

	void BloomComputePass::DrawScene(Scene& scene) const
	{
		m_Prefilter->Bind();
		BindInputs();
		//m_Half->BindImage(1);
		m_Textures[0]->BindImage(1);
		m_Prefilter->Dispatch(m_Textures[0]->GetWidth(), m_Textures[0]->GetHeight(), 1);
		m_Prefilter->Barrier();

		m_DownsampleVertical->Bind();
		m_Textures[0]->BindImage(0);
		m_Textures[0]->BindImage(1);
		m_DownsampleVertical->Dispatch(m_Textures[0]->GetWidth(), m_Textures[0]->GetHeight(), 1);
		m_DownsampleVertical->Barrier();

		for (uint32_t i = 1; i < m_Passes; i++) {
			m_Textures[i]->BindImage(1);
			m_DownsampleHorizontal->Bind();
			m_DownsampleHorizontal->Dispatch(m_Textures[i]->GetWidth(), m_Textures[i]->GetHeight(), 1);
			m_DownsampleHorizontal->Barrier();
			m_Textures[i]->BindImage(0);
			m_DownsampleVertical->Bind();
			m_DownsampleVertical->Dispatch(m_Textures[i]->GetWidth(), m_Textures[i]->GetHeight(), 1);
			m_DownsampleVertical->Barrier();
		}

		/*m_Quarter->BindImage(1);
		m_DownsampleHorizontal->Bind();
		m_DownsampleHorizontal->Dispatch(m_Quarter->GetWidth(), m_Quarter->GetHeight(), 1);
		m_DownsampleHorizontal->Barrier();
		m_Quarter->BindImage(0);
		m_DownsampleVertical->Bind();
		m_DownsampleVertical->Dispatch(m_Quarter->GetWidth(), m_Quarter->GetHeight(), 1);
		m_DownsampleVertical->Barrier();

		m_Eigth->BindImage(1);
		m_DownsampleHorizontal->Bind();
		m_DownsampleHorizontal->Dispatch(m_Eigth->GetWidth(), m_Eigth->GetHeight(), 1);
		m_DownsampleHorizontal->Barrier();
		m_Eigth->BindImage(0);
		m_DownsampleVertical->Bind();
		m_DownsampleVertical->Dispatch(m_Eigth->GetWidth(), m_Eigth->GetHeight(), 1);
		m_DownsampleVertical->Barrier();*/

		for (uint32_t i = m_Passes - 1; i >= 0; i--) {
			W_CORE_ERROR("{0}", i);
			m_Textures[i]->BindImage(1);
			m_DownsampleHorizontal->Bind();
			m_DownsampleHorizontal->Dispatch(m_Textures[i]->GetWidth(), m_Textures[i]->GetHeight(), 1);
			m_DownsampleHorizontal->Barrier();
			m_Textures[i]->BindImage(0);
			m_DownsampleVertical->Bind();
			m_DownsampleVertical->Dispatch(m_Textures[i]->GetWidth(), m_Textures[i]->GetHeight(), 1);
			m_DownsampleVertical->Barrier();

			if (i == 0) {
				break;
			}
		}

		/*m_Quarter->BindImage(1);
		m_DownsampleHorizontal->Bind();
		m_DownsampleHorizontal->Dispatch(m_Quarter->GetWidth(), m_Quarter->GetHeight(), 1);
		m_DownsampleHorizontal->Barrier();
		m_Quarter->BindImage(0);
		m_DownsampleVertical->Bind();
		m_DownsampleVertical->Dispatch(m_Quarter->GetWidth(), m_Quarter->GetHeight(), 1);
		m_DownsampleVertical->Barrier();

		m_Half->BindImage(1);
		m_DownsampleHorizontal->Bind();
		m_DownsampleHorizontal->Dispatch(m_Half->GetWidth(), m_Half->GetHeight(), 1);
		m_DownsampleHorizontal->Barrier();
		m_Half->BindImage(0);
		m_DownsampleVertical->Bind();
		m_DownsampleVertical->Dispatch(m_Half->GetWidth(), m_Half->GetHeight(), 1);
		m_DownsampleVertical->Barrier();*/
	}
	std::shared_ptr<Framebuffer> BloomComputePass::GetFramebuffer() const
	{
		return nullptr;
	}

	uint32_t BloomComputePass::GetFinalOutputRendererId() const
	{
		return m_Half->GetRendererID();
	}

	void BloomComputePass::Resize(uint32_t width, uint32_t height)
	{

	}
}