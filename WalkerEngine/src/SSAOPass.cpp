#include "walkerpch.h"
#include "SSAOPass.h"
#include <glad/glad.h>

SSAOPass::SSAOPass(unsigned int width, unsigned int height)
{
	InitBuffers(width, height);
	InitKernel();
	InitNoiseTexture();
}

void SSAOPass::InitBuffers(unsigned int width, unsigned int height)
{
	// Framebuffer
	glGenFramebuffers(1, &ssaoFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

	// Color buffer texture
	glGenTextures(1, &ssaoColorBuffer);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED,
		GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
}

void SSAOPass::InitKernel()
{
	for (unsigned int i = 0; i < 64; ++i)
	{
		glm::vec3 sample(
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator)
		);
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);
		float scale = (float)i / 64.0;
		scale = lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;
		ssaoKernel.push_back(sample);
	}

	for (unsigned int i = 0; i < 16; i++)
	{
		glm::vec3 noise(
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator) * 2.0 - 1.0,
			0.0f);
		ssaoNoise.push_back(noise);
	}
}

void SSAOPass::InitNoiseTexture()
{
	glGenTextures(1, &noiseTexture);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 4, 4, 0, GL_RGB, GL_FLOAT,
		&ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void SSAOPass::BindFramebuffer(bool clearBuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
	if (clearBuffer) {
		glClear(GL_COLOR_BUFFER_BIT);
	}
}

void SSAOPass::Draw(glm::mat4 projection, glm::mat4 view)
{
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	shaderSSAO.use();

	for (int i = 0; i < ssaoKernel.size(); i++) {
		shaderSSAO.setVec3("samples[" + std::to_string(i) + "]", ssaoKernel[i]);
	}
	shaderSSAO.setMat4("projection", projection);
	shaderSSAO.setMat4("view", view);
	shaderSSAO.setInt("gPosition", 0);
	shaderSSAO.setInt("gNormal", 1);
	shaderSSAO.setInt("texNoise", 2);

	quad.Draw();
}

void SSAOPass::BindTextures()
{
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
}

float SSAOPass::lerp(float a, float b, float f)
{
	return a + f * (b - a);
}
