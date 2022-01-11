#pragma once
#include <glad/glad.h>
#include "Shader.h"
#include "Model.h"

class GBufferPBR {
public:
	GBufferPBR(unsigned int width, unsigned int height);
	void InitBuffers(unsigned int width, unsigned int height);
	void BindTextures();
	void DrawModel(Model& model, glm::mat4 view, glm::mat4 projection);
	void BindFramebuffer();
	void ControlWindow();
	unsigned int GetGBuffer();
	int debugPass = 0;
	bool ambientOcclusionEnabled = true;

private:
	unsigned int gBuffer, gPosition, gNormal, gAlbedo, gMetRoughAO;
	unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	unsigned int rboDepth;
	Shader shader = Shader("Shaders/g_buffer.vert", "Shaders/g_buffer_pbr.frag");
};