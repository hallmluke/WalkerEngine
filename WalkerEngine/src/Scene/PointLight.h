#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "Renderer/Framebuffer.h"


namespace Walker {

	class Shader;
	class Model;
	class Scene;

	class PointLight {
	public:
		PointLight();

		//glm::vec3 GetPosition() const { return m_Position; }
		float GetAmbientIntensity() const { return m_AmbientIntensity; }
		float GetDiffuseIntensity() const { return m_DiffuseIntensity; }
		float GetSpecularIntensity() const { return m_SpecularIntensity; }

		float GetConstantAttenuation() const { return m_ConstantAttenuation; }
		float GetLinearAttenuation() const { return m_LinearAttenuation; }
		float GetQuadraticAttenuation() const { return m_QuadraticAttenuation; }
		//void DrawDebug(Shader& shader);

		void GenerateShadowMap(Scene& scene, glm::vec3 position);
		void BindShadowMap(uint32_t slot);
		float GetShadowMapFarPlane() const { return m_ShadowMapFarPlane; }

	private:
		float m_AmbientIntensity;
		float m_DiffuseIntensity;
		float m_SpecularIntensity;

		float m_ConstantAttenuation;
		float m_LinearAttenuation;
		float m_QuadraticAttenuation;

		// Shadow mapping
		bool m_ShadowMapEnabled = true;
		std::shared_ptr<Framebuffer> m_ShadowMapFramebuffer;
		uint32_t m_ShadowMapWidth = 1024;
		uint32_t m_ShadowMapHeight = 1024;
		float m_ShadowMapNearPlane = 0.1f;
		float m_ShadowMapFarPlane = 25.0f;
		glm::mat4 m_ShadowMapProjection = glm::perspective(glm::radians(90.0f), 1.0f, m_ShadowMapNearPlane, m_ShadowMapFarPlane);
		std::vector<glm::mat4> m_ShadowTransforms;
		std::shared_ptr<Shader> m_ShadowMapShader;

		void UpdateShadowTransforms(glm::vec3 position);

		friend class SceneHierarchyPanel;
		friend class SceneSerializer;

		// TEMP
		friend class SandboxLayer;
	};
}