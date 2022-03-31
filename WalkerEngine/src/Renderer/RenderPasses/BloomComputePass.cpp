#include "walkerpch.h"
#include "BloomComputePass.h"

namespace Walker {

	BloomComputePass::BloomComputePass(uint32_t width, uint32_t height)
	{
		m_Passes = 5;
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
		//m_Textures.push_back(Texture2D::Create(spec));
		for (uint32_t i = 1; i < m_Passes; i++) {
			spec.Width = spec.Width / 2;
			spec.Height = spec.Height / 2;
			m_Textures.push_back(Texture2D::Create(spec));
			//m_Textures.push_back(Texture2D::Create(spec));
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
		m_Outputs = { { "img_output", 1, this } };
		m_DownsampleHorizontal = ComputeShader::Create("DownsampleHorizontal", "Shaders/gaussian_downsample_horizontal.comp");
		m_DownsampleVertical = ComputeShader::Create("DownsampleVertical", "Shaders/gaussian_blur_vertical.comp");
		//m_Downsample = ComputeShader::Create("Downsample", "Shaders/downsample.comp");
		m_Upsample = ComputeShader::Create("Upsample", "Shaders/bloom_upsample.comp");
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
		m_Textures[0]->Bind(inputSlot);
		//m_Half->Bind(inputSlot);
		//m_Quarter->Bind(inputSlot);
		//m_Eigth->Bind(inputSlot);
	}

	void BloomComputePass::BindOutputImage(uint32_t outputSlot, uint32_t inputSlot) const
	{
		m_Textures[0]->BindImage(inputSlot);
		//m_Half->BindImage(inputSlot);
		//m_Eigth->BindImage(inputSlot);
	}

	void BloomComputePass::DrawScene(Scene& scene) const
	{
		m_Prefilter->Bind();
		BindInputs();
		//m_Half->BindImage(1);
		m_Textures[0]->BindImage(1);
		m_Prefilter->Dispatch((m_Textures[0]->GetWidth() / 32) + 1, (m_Textures[0]->GetHeight() / 32) + 1, 1);
		m_Prefilter->Barrier();

		m_DownsampleVertical->Bind();
		m_Textures[0]->BindImage(0);
		//m_Textures[1]->BindImage(1);
		m_DownsampleVertical->Dispatch((m_Textures[0]->GetWidth() / 32) + 1, (m_Textures[0]->GetHeight() / 32) + 1, 1);
		m_DownsampleVertical->Barrier();

		/*for (uint32_t i = 2; i < m_Textures.size() - 1; i = i + 2) {
			m_Textures[i]->BindImage(1);
			m_DownsampleHorizontal->Bind();
			m_DownsampleHorizontal->Dispatch((m_Textures[i]->GetWidth() / 32) + 1, (m_Textures[i]->GetHeight() / 32) + 1, 1);
			m_DownsampleHorizontal->Barrier();
			m_Textures[i]->BindImage(0);
			m_Textures[i + 1]->BindImage(1);
			m_DownsampleVertical->Bind();
			m_DownsampleVertical->Dispatch((m_Textures[i]->GetWidth() / 32) + 1, (m_Textures[i]->GetHeight() / 32) + 1, 1);
			m_DownsampleVertical->Barrier();
		}*/

		for (uint32_t i = 1; i < m_Textures.size(); i++) {
			m_Textures[i]->BindImage(1);
			m_DownsampleHorizontal->Bind();
			m_DownsampleHorizontal->Dispatch((m_Textures[i]->GetWidth() / 32) + 1, (m_Textures[i]->GetHeight() / 32) + 1, 1);
			m_DownsampleHorizontal->Barrier();
			m_Textures[i]->BindImage(0);
			m_DownsampleVertical->Bind();
			m_DownsampleVertical->Dispatch((m_Textures[i]->GetWidth() / 32) + 1, (m_Textures[i]->GetHeight() / 32) + 1, 1);
			m_DownsampleVertical->Barrier();
		}


		/*for (uint32_t i = m_Textures.size() - 1;  i != std::numeric_limits<uint32_t>::max() && i != std::numeric_limits<uint32_t>::max() - 1; i = i - 2) {
			//W_CORE_ERROR("{0}", i);
			m_Textures[i]->BindImage(1);
			m_DownsampleHorizontal->Bind();
			m_DownsampleHorizontal->Dispatch((m_Textures[i]->GetWidth() / 32) + 1, (m_Textures[i]->GetHeight() / 32) + 1, 1);
			m_DownsampleHorizontal->Barrier();
			m_Textures[i]->BindImage(0);
			m_Textures[i - 1]->BindImage(1);
			m_DownsampleVertical->Bind();
			m_DownsampleVertical->Dispatch((m_Textures[i]->GetWidth() / 32) + 1, (m_Textures[i]->GetHeight() / 32) + 1, 1);
			m_DownsampleVertical->Barrier();

		}*/

		m_Textures[m_Textures.size() - 1]->Bind(0);

		for (uint32_t i = m_Textures.size() - 1; i != std::numeric_limits<uint32_t>::max(); i--) {
			//W_CORE_ERROR("{0}", i);
			m_Textures[i]->BindImage(1);
			
			
			m_Upsample->Bind();
			m_Upsample->SetInt("input_sampler", 0);
			m_Upsample->Dispatch((m_Textures[i]->GetWidth() / 32) + 1, (m_Textures[i]->GetHeight() / 32) + 1, 1);
			m_Upsample->Barrier();
			m_Textures[i]->BindImage(0);
			m_Textures[i]->Bind(0);
			/*m_DownsampleHorizontal->Bind();
			m_DownsampleHorizontal->Dispatch((m_Textures[i]->GetWidth() / 32) + 1, (m_Textures[i]->GetHeight() / 32) + 1, 1);
			m_DownsampleHorizontal->Barrier();
			m_Textures[i]->BindImage(0);
			//m_Textures[i - 1]->BindImage(1);
			m_DownsampleVertical->Bind();
			m_DownsampleVertical->Dispatch((m_Textures[i]->GetWidth() / 32) + 1, (m_Textures[i]->GetHeight() / 32) + 1, 1);
			m_DownsampleVertical->Barrier();*/

		}

	}
	std::shared_ptr<Framebuffer> BloomComputePass::GetFramebuffer() const
	{
		return nullptr;
	}

	uint32_t BloomComputePass::GetFinalOutputRendererId() const
	{
		return m_Textures[0]->GetRendererID();
	}

	void BloomComputePass::Resize(uint32_t width, uint32_t height)
	{

	}
}