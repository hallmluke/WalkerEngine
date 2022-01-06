#pragma once

class Quad {
public:
	Quad();
	void InitBuffers();
	void Draw();

private:
	unsigned int quadVAO = 0;
	unsigned int quadVBO;
};