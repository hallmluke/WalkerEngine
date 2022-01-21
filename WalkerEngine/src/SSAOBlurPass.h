#pragma once
#include "Quad.h"
#include "Shader.h"

class SSAOBlurPass {
public:
	SSAOBlurPass(unsigned int width, unsigned int height);
	void InitBuffers(unsigned int width, unsigned int height);
	void BindFrameBuffer(bool clearBuffer = true);
	void Draw();
	void BindTextures();

private:
	unsigned int ssaoBlurFBO, ssaoColorBufferBlur;
	Quad quad;
	Shader shaderSSAOBlur = Shader("Shaders/ssao_pass.vert", "Shaders/ssao_blur_pass.frag");
};