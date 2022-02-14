#include "walkerpch.h"
#include "PointLight.h"

#include "Model.h"
#include "Renderer/Shader.h"
#include "imgui/imgui.h"


namespace Walker {

	PointLight::PointLight(glm::vec3 pos) : m_Position(pos)
	{
		m_Name = "Point Light";
		m_AmbientIntensity = 0.1f;
		m_DiffuseIntensity = 0.8f;
		m_SpecularIntensity = 1.0f;

		m_ConstantAttenuation = 1.0f;
		m_LinearAttenuation = 0.09f;
		m_QuadraticAttenuation = 0.032f;

		FramebufferSpecification fbSpec;
		fbSpec.Attachments = {
			{ "Depth", FramebufferTextureFormat::DEPTH32F, FramebufferTextureType::FLOAT, FramebufferTextureTarget::TEXTURE_CUBE_MAP }
		};
		fbSpec.Width = m_ShadowMapWidth;
		fbSpec.Height = m_ShadowMapHeight;
		fbSpec.Depth = 1;
		fbSpec.Samples = 1;
		m_ShadowMapFramebuffer = Framebuffer::Create(fbSpec);

		m_ShadowMapShader = Shader::Create("DepthCubeShader", "Shaders/depth_shader_cube.vert", "Shaders/depth_shader_cube.frag", "Shaders/depth_shader_cube.geom");

		UpdateShadowTransforms();
	}

	void PointLight::GenerateShadowMap(Model& model)
	{
		m_ShadowMapFramebuffer->Bind();
		m_ShadowMapShader->Bind();

		for (uint32_t i = 0; i < m_ShadowTransforms.size(); i++) {
			m_ShadowMapShader->SetMat4("shadowMatrices[" + std::to_string(i) + "]", m_ShadowTransforms[i]);
		}
		m_ShadowMapShader->SetFloat("far_plane", m_ShadowMapFarPlane);
		m_ShadowMapShader->SetVec3("lightPos", m_Position);
		RenderCommand::Clear();
		model.Draw(m_ShadowMapShader);
	}

	void PointLight::BindShadowMap(uint32_t slot)
	{
		m_ShadowMapFramebuffer->BindDepthAttachment(slot);
	}

	void PointLight::UpdateShadowTransforms()
	{
		std::vector<glm::mat4> shadowTransforms;

		shadowTransforms.push_back(m_ShadowMapProjection *
			glm::lookAt(m_Position, m_Position + glm::vec3(1.0, 0.0, 0.0)
				, glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(m_ShadowMapProjection *
			glm::lookAt(m_Position, m_Position + glm::vec3(-1.0, 0.0, 0.0)
				, glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(m_ShadowMapProjection *
			glm::lookAt(m_Position, m_Position + glm::vec3(0.0, 1.0, 0.0)
				, glm::vec3(0.0, 0.0, 1.0)));
		shadowTransforms.push_back(m_ShadowMapProjection *
			glm::lookAt(m_Position, m_Position + glm::vec3(0.0, -1.0, 0.0)
				, glm::vec3(0.0, 0.0, -1.0)));
		shadowTransforms.push_back(m_ShadowMapProjection *
			glm::lookAt(m_Position, m_Position + glm::vec3(0.0, 0.0, 1.0)
				, glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(m_ShadowMapProjection *
			glm::lookAt(m_Position, m_Position + glm::vec3(0.0, 0.0, -1.0)
				, glm::vec3(0.0, -1.0, 0.0)));

		m_ShadowTransforms = shadowTransforms;
	}

	void PointLight::ControlWindow()
	{
		if (ImGui::Begin(m_Name.c_str())) {
			ImGui::Text("Position");
			if (ImGui::SliderFloat("X", &m_Position.x, -20.0f, 20.0f)) {
				UpdateShadowTransforms();
			}
			if (ImGui::SliderFloat("Y", &m_Position.y, -20.0f, 20.0f)) {
				UpdateShadowTransforms();
			}
			if (ImGui::SliderFloat("Z", &m_Position.z, -20.0f, 20.0f)) {
				UpdateShadowTransforms();
			}

			ImGui::Text("Intensity/Color");
			ImGui::SliderFloat("Ambient Intensity", &m_AmbientIntensity, 0.01f, 0.5f);
			ImGui::SliderFloat("Diffuse Intensity", &m_DiffuseIntensity, 0.4f, 1.0f);
			ImGui::SliderFloat("Specular Intensity", &m_SpecularIntensity, 0.8f, 1.2f);

			ImGui::Text("Falloff");
			ImGui::SliderFloat("Constant", &m_ConstantAttenuation, 0.05f, 10.0f);
			ImGui::SliderFloat("Linear", &m_LinearAttenuation, 0.0001f, 4.0f);
			ImGui::SliderFloat("Quadratic", &m_QuadraticAttenuation, 0.0000001f, 0.35f);

			//ImGui::Text("Shadow Map");
			//ImGui::SliderFloat("Bias", &bias, 0.001f, 0.1f);
			//
			//ImGui::Text("Debug");
			//ImGui::Checkbox("Draw", &drawDebugEnabled);
			//ImGui::SliderFloat("Light scale", &debugDrawSize, 0.01f, 3.0f);
			//ImGui::Checkbox("Debug Shadow", &debugShadow);
		}
		ImGui::End();
	}

	/*void PointLight::DrawDebug(std::shared_ptr<Shader> shader)
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

			ImGui::Text("Shadow Map");
			ImGui::SliderFloat("Bias", &bias, 0.001f, 0.1f);

			ImGui::Text("Debug");
			ImGui::Checkbox("Draw", &drawDebugEnabled);
			ImGui::SliderFloat("Light scale", &debugDrawSize, 0.01f, 3.0f);
			ImGui::Checkbox("Debug Shadow", &debugShadow);
		}
		ImGui::End();
	}

	void PointLight::GenerateShadowMap(Shader& shader, Model& model)
	{
		if (!shadowMapInitialized) {
			InitShadowMap();
		}

		// Get current viewport size to reset after
		GLint m_viewport[4];
		glGetIntegerv(GL_VIEWPORT, m_viewport);

		std::vector<glm::mat4> shadowTransforms;

		shadowTransforms.push_back(shadowProj *
			glm::lookAt(position, position + glm::vec3(1.0, 0.0, 0.0)
				, glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(position, position + glm::vec3(-1.0, 0.0, 0.0)
				, glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(position, position + glm::vec3(0.0, 1.0, 0.0)
				, glm::vec3(0.0, 0.0, 1.0)));
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(position, position + glm::vec3(0.0, -1.0, 0.0)
				, glm::vec3(0.0, 0.0, -1.0)));
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(position, position + glm::vec3(0.0, 0.0, 1.0)
				, glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(position, position + glm::vec3(0.0, 0.0, -1.0)
				, glm::vec3(0.0, -1.0, 0.0)));

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		shader.use();
		for (unsigned int i = 0; i < 6; ++i) {
			shader.setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
		}
		shader.setFloat("far_plane", farPlane);
		shader.setVec3("lightPos", position);
		//shader.setMat4("model", model.transform);

		model.Draw(shader);

		// Reset framebuffer, viewport
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void PointLight::InitBuffers()
	{
		vertices = Cube::getUnindexedVertices();

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);
		glBindVertexArray(VAO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);

		initialized = true;
	}

	void PointLight::InitShadowMap()
	{
		glGenFramebuffers(1, &depthMapFBO);
		glGenTextures(1, &depthCubeMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
		for (unsigned int i = 0; i < 6; ++i) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
				SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT,
				GL_FLOAT, NULL);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		shadowMapInitialized = true;
	}
}*/
}