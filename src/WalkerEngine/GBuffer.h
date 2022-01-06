#pragma once
#include <glad/glad.h>
#include "Shader.h"
#include "Model.h"

class GBuffer {
public:
	GBuffer(unsigned int width, unsigned int height);
	void InitBuffers(unsigned int width, unsigned int height);
	void BindTextures();
	void DrawModel(Model& model, glm::mat4 view, glm::mat4 projection);
	void BindFramebuffer();
	void ControlWindow();
	int debugPass = 0;

private:
	unsigned int gBuffer, gPosition, gNormal, gAlbedoSpec;
	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	unsigned int rboDepth;
	Shader shader = Shader("Shaders/g_buffer.vert", "Shaders/g_buffer.frag");
};