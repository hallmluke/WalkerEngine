#pragma once
#include <random>
#include <glm/glm.hpp>

#include "Math/SampleGeometry/Quad.h"
#include "Renderer/RenderPass.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/Framebuffer.h"

namespace Walker {

	class SSAOPass : RenderPass {
	public:
		SSAOPass(uint32_t width, uint32_t height);
		//void InitBuffers(unsigned int width, unsigned int height);
		void InitKernel();
		void InitNoiseTexture();
		//void BindFramebuffer(bool clearBuffer = true);
		virtual void BindInputs() const override;
		virtual void Draw() const override;
		virtual std::vector<RenderPassInput> GetInputs() const override { return m_Inputs; };
		//void BindTextures();

	private:
		std::shared_ptr<Framebuffer> m_Framebuffer;
		std::shared_ptr<Shader> m_Shader;
		std::vector<RenderPassInput> m_Inputs = {};
		//unsigned int ssaoFBO, ssaoColorBuffer;
		//unsigned int noiseTexture;
		//Quad quad;
		//Shader shaderSSAO = Shader("Shaders/ssao_pass.vert", "Shaders/ssao_pass.frag");
		std::uniform_real_distribution<float> m_RandomFloats = std::uniform_real_distribution<float>(0.0f, 1.0f);
		std::default_random_engine m_Generator;
		std::vector<glm::vec3> m_SSAOKernel;
		std::vector<glm::vec3> m_SSAONoise;
		//float lerp(float a, float b, float f);
	};
}