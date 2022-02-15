#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer/Framebuffer.h"
#include "Renderer/UniformBuffer.h"

namespace Walker {

	class Shader;
	class Scene;
	class Camera;

	class DirectionalLight {
	public:
		DirectionalLight(glm::vec3 dir, bool shadowMapEnabled = true);

		glm::vec3 GetDirection() const { return m_Direction; }
		glm::vec3 GetColor() const { return m_Color; }
		float GetAmbientIntensity() const { return m_AmbientIntensity; }
		float GetDiffuseIntensity() const { return m_DiffuseIntensity; }
		float GetSpecularIntensity() const { return m_SpecularIntensity; }
		
		// Shadow mapping
		glm::mat4 GetLightSpaceMatrix(Camera& camera, const float nearPlane, const float farPlane);
		std::vector<glm::mat4> GetLightSpaceMatrices(Camera& camera);

		std::vector<float> GetShadowCascadeLevels() const { return m_ShadowCascadeLevels; };
		void SetShadowCascadeLevels(const float cameraFarPlane);
		
		void GenerateCascadedShadowMap(Scene& scene);
		void BindShadowMap(uint32_t slot) const;


	private:
		std::string m_Name;
		glm::vec3 m_Direction;
		glm::vec3 m_Color;
		float m_Distance = 20.0f;

		float m_AmbientIntensity;
		float m_DiffuseIntensity;
		float m_SpecularIntensity;

		// Shadow mapping
		bool m_ShadowMapEnabled = true;
		std::shared_ptr<Framebuffer> m_ShadowMapFramebuffer;
		std::shared_ptr<UniformBuffer> m_LightMatricesUniformBuffer;
		uint32_t m_ShadowMapWidth = 2048;
		uint32_t m_ShadowMapHeight = 2048;
		std::vector<float> m_ShadowCascadeLevels;
		std::shared_ptr<Shader> m_ShadowMapShader;

		float debugDrawSize = 0.2f;
		bool drawDebugEnabled = true;
		float debugDistance = 20.0f;
	};
}