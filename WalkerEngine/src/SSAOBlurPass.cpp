#include "walkerpch.h"
#include "SSAOBlurPass.h"

SSAOBlurPass::SSAOBlurPass(unsigned int width, unsigned int height)
{
	InitBuffers(width, height);
}

void SSAOBlurPass::InitBuffers(unsigned int width, unsigned int height)
{
	// Frame buffer
	glGenFramebuffers(1, &ssaoBlurFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);

	// Color buffer
	glGenTextures(1, &ssaoColorBufferBlur);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED,
		GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);
}

void SSAOBlurPass::BindFrameBuffer(bool clearBuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
	if (clearBuffer) {
		glClear(GL_COLOR_BUFFER_BIT);
	}
}

void SSAOBlurPass::Draw()
{
	shaderSSAOBlur.use();
	shaderSSAOBlur.setInt("ssaoInput", 3);
	quad.Draw();
}

void SSAOBlurPass::BindTextures()
{
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
}
