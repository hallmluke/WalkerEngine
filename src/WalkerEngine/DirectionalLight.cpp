#include "DirectionalLight.h"
#include "Cube.h"
#include "Shader.h"
#include "imgui\imgui.h";

DirectionalLight::DirectionalLight(glm::vec3 dir) : direction(dir)
{
	name = "Directional Light";
	ambientIntensity = 0.1f;
	diffuseIntensity = 0.8f;
	specularIntensity = 1.0f;

}

void DirectionalLight::DrawDebug(Shader& shader)
{
	if (!initialized) {
		InitBuffers();
	}

	glBindVertexArray(VAO);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, direction * -debugDistance);
	model = glm::scale(model, glm::vec3(debugDrawSize));
	shader.setMat4("model", model);

	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void DirectionalLight::ControlWindow()
{
	if (ImGui::Begin(name)) {
		ImGui::Text("Direction");
		ImGui::SliderFloat("X", &direction.x, -1.0f, 1.0f);
		ImGui::SliderFloat("Y", &direction.y, -1.0f, 1.0f);
		ImGui::SliderFloat("Z", &direction.z, -1.0f, 1.0f);

		ImGui::Text("Intensity");
		ImGui::SliderFloat("Ambient Intensity", &ambientIntensity, 0.01f, 0.5f);
		ImGui::SliderFloat("Diffuse Intensity", &diffuseIntensity, 0.4f, 1.0f);
		ImGui::SliderFloat("Specular Intensity", &specularIntensity, 0.8f, 1.2f);

		ImGui::Text("Debug");
		ImGui::Checkbox("Draw", &drawDebugEnabled);
		ImGui::SliderFloat("Light scale", &debugDrawSize, 0.01f, 3.0f);
		ImGui::SliderFloat("Distance", &debugDistance, 1.0f, 40.0f);
	}
	ImGui::End();
}

void DirectionalLight::InitBuffers()
{
	vector<float> vertices = Cube::getUnindexedVertices();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Cube::CubeVertex), &vertices[0], GL_STATIC_DRAW);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	initialized = true;
}
