#include "walkerpch.h"
#include "Volume.h"
#include "Cube.h"
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "imgui/imgui.h"

Volume::Volume()
{
	InitBuffers();
	PopulateTestData();
	InitTexture();
}

void Volume::InitBuffers()
{
	vector<float> vertices = Cube::getUnindexedVertices();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
}

void Volume::InitTexture()
{
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_3D, textureId);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, size, size, size, 0, GL_RED, GL_UNSIGNED_BYTE, &data[0]);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glGenerateMipmap(GL_TEXTURE_3D);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_LEVEL, 0);
}

void Volume::Draw(glm::mat4 view, glm::mat4 projection, glm::vec3 viewPos)
{
	glBindVertexArray(VAO);

	glm::mat4 model = glm::mat4(1.0f);
	raymarchShader.use();
	raymarchShader.setMat4("model", model);
	raymarchShader.setMat4("view", view);
	raymarchShader.setMat4("projection", projection);
	raymarchShader.setMat4("inverseModel", glm::inverse(model));
	raymarchShader.setVec3("viewPos", viewPos);
	raymarchShader.setInt("steps", steps);
	raymarchShader.setFloat("stepSize", stepSize);
	raymarchShader.setFloat("radius", radius);
	raymarchShader.setFloat("opacityThreshold", opacityThreshold);
	raymarchShader.setVec3("origin", origin);

	GLuint texLoc = glGetUniformLocation(raymarchShader.ID, "volumeTex");
	glUniform1i(texLoc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, textureId);
	//raymarchShader.setInt("volumeTex", 1);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	//quad.Draw();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Volume::ControlWindow()
{
	if (ImGui::Begin("Volume")) {
		ImGui::SliderFloat("Step Size", &stepSize, 0.01, 0.5);
		ImGui::SliderInt("Steps", &steps, 0, 50);
		ImGui::SliderFloat("Radius", &radius, 0.01, 2.0);
		ImGui::SliderFloat("Opacity Threshold", &opacityThreshold, 0.0, 1.0);
		ImGui::SliderFloat("X", &origin.x, -2.0, 2.0);
		ImGui::SliderFloat("Y", &origin.y, -2.0, 2.0);
		ImGui::SliderFloat("Z", &origin.z, -2.0, 2.0);
	}

	ImGui::End();
}

void Volume::PopulateTestData()
{
	data.reserve(size * size * size);
	glm::vec3 centerPoint = glm::vec3(size / 2, size / 2, size / 2);
	float radius = 40;

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			for (int k = 0; k < size; k++) {

				glm::vec3 currentPoint = glm::vec3(i, j, k);

				glm::vec3 centerToCurrent = centerPoint - currentPoint;

				float length = glm::distance(centerPoint, currentPoint);

				if (length <= radius) {
					data.push_back((uint8_t) 255);
				}
				else {
					data.push_back((uint8_t) 0);
				}
				
			}
		}
	}
}
