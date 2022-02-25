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

		//float debugDrawSize = 0.2f;
		//bool drawDebugEnabled = true;
		//bool debugShadow = false;

		//unsigned int VBO, VAO;
		//std::vector<float> vertices;

		// Shadow mapping
		/*bool shadowMapEnabled = true;
		unsigned int depthCubeMap;
		unsigned int depthMapFBO;
		float nearPlane = 0.1f;
		float farPlane = 25.0f;
		float aspect = 1.0f;
		float bias = 0.05f;
		glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, nearPlane, farPlane);
		void GenerateShadowMap(Shader& shader, Model& model);*/

	private:
		std::string m_Name;

		//glm::vec3 m_Position;
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

		/*bool initialized = false;
		void InitBuffers();

		// Shadow mapping
		const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
		bool shadowMapInitialized = false;
		void InitShadowMap();*/
	};
}