#include "PointLight.h"
#include "Cube.h"
#include "Shader.h"
#include "imgui\imgui.h";

PointLight::PointLight(glm::vec3 pos) : position(pos)
{
	name = "Point Light";
	ambientIntensity = 0.1f;
	diffuseIntensity = 0.8f;
	specularIntensity = 1.0f;

	constantAttenuation = 1.0f;
	linearAttenuation = 0.09f;
	quadraticAttenuation = 0.032f;
}

void PointLight::DrawDebug(Shader& shader)
{
	if (!initialized) {
		InitBuffers();
	}

	glBindVertexArray(VAO);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::scale(model, glm::vec3(debugDrawSize));
	shader.setMat4("model", model);

	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void PointLight::ControlWindow()
{
	if (ImGui::Begin(name)) {
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &position.x, -20.0f, 20.0f);
		ImGui::SliderFloat("Y", &position.y, -20.0f, 20.0f);
		ImGui::SliderFloat("Z", &position.z, -20.0f, 20.0f);

		ImGui::Text("Intensity/Color");
		ImGui::SliderFloat("Ambient Intensity", &ambientIntensity, 0.01f, 0.5f);
		ImGui::SliderFloat("Diffuse Intensity", &diffuseIntensity, 0.4f, 1.0f);
		ImGui::SliderFloat("Specular Intensity", &specularIntensity, 0.8f, 1.2f);

		ImGui::Text("Falloff");
		ImGui::SliderFloat("Constant", &constantAttenuation, 0.05f, 10.0f);
		ImGui::SliderFloat("Linear", &linearAttenuation, 0.0001f, 4.0f);
		ImGui::SliderFloat("Quadratic", &quadraticAttenuation, 0.0000001f, 10.0f);

		ImGui::Text("Debug");
		ImGui::Checkbox("Draw", &drawDebugEnabled);
		ImGui::SliderFloat("Light scale", &debugDrawSize, 0.01f, 3.0f);
	}
	ImGui::End();
}

void PointLight::InitBuffers()
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
