#pragma once
#include <random>
#include <glm/glm.hpp>

#include "Quad.h"
#include "Shader.h"

class SSAOPass {
public:
	SSAOPass(unsigned int width, unsigned int height);
	void InitBuffers(unsigned int width, unsigned int height);
	void InitKernel();
	void InitNoiseTexture();
	void BindFramebuffer(bool clearBuffer = true);
	void Draw(glm::mat4 projection, glm::mat4 view);
	void BindTextures();

private:
	unsigned int ssaoFBO, ssaoColorBuffer;
	unsigned int noiseTexture;
	Quad quad;
	Shader shaderSSAO = Shader("Shaders/ssao_pass.vert", "Shaders/ssao_pass.frag");
	std::uniform_real_distribution<float> randomFloats = std::uniform_real_distribution<float>(0.0f, 1.0f);
	std::default_random_engine generator;
	std::vector<glm::vec3> ssaoKernel;
	std::vector<glm::vec3> ssaoNoise;
	float lerp(float a, float b, float f);
};