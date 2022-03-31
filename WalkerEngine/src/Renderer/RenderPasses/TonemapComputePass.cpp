#include "walkerpch.h"
#include "TonemapComputePass.h"

namespace Walker {
	TonemapComputePass::TonemapComputePass(uint32_t width, uint32_t height)
	{
		TextureSpecification spec;
		spec.Width = width;
		spec.Height = height;
		spec.MagFilter = TextureFilterType::LINEAR;
		spec.MinFilter = TextureFilterType::LINEAR;
		spec.Type = TextureType::FLOAT;
		spec.TextureFormat = TextureFormat::RGBA16F;
		spec.WrapS = TextureWrapType::CLAMP_EDGE;
		spec.WrapT = TextureWrapType::CLAMP_EDGE;

		m_Output = Texture2D::Create(spec);

		m_TonemapShader = ComputeShader::Create("Tonemap", "Shaders/tonemap.comp");

		m_Inputs = { { "img_input", 0, this }, {"bloom_input", 1, this } };
		m_Outputs = { {"img_output", 2, this} };
	}

	void TonemapComputePass::BindInputs() const
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
	void TonemapComputePass::BindOutput(uint32_t outputSlot, uint32_t inputSlot) const
	{
		m_Output->Bind(inputSlot);
	}
	void TonemapComputePass::BindOutputImage(uint32_t outputSlot, uint32_t inputSlot) const
	{
		m_Output->BindImage(inputSlot);
	}

	void TonemapComputePass::DrawScene(Scene& scene) const
	{
		m_TonemapShader->Bind();
		m_TonemapShader->Barrier();
		BindInputs();
		m_Output->BindImage(2);
		m_TonemapShader->Dispatch(m_Output->GetWidth() / 32, m_Output->GetHeight() / 32, 1);
		m_TonemapShader->Barrier();
	}
	std::shared_ptr<Framebuffer> TonemapComputePass::GetFramebuffer() const
	{
		return nullptr;
	}
	uint32_t TonemapComputePass::GetFinalOutputRendererId() const
	{
		return m_Output->GetRendererID();
	}
	void TonemapComputePass::Resize(uint32_t width, uint32_t height)
	{
		// TODO: Resize for comp shaders
	}
}