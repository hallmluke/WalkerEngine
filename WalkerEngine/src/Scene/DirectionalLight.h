#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer/Framebuffer.h"
#include "Renderer/UniformBuffer.h"

namespace Walker {

	class Shader;
	class Scene;
	class Camera;

	struct GlobalShadow {
		glm::mat4 WorldToShadowSpace;
		glm::mat4 ShadowView;
		float Radius;
	};

	struct BoundingSphere {
		glm::vec3 FrustumCenter;
		float Radius;
	};

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
		glm::mat4 GetLightSpaceMatrix(Camera& camera, const float nearPlane, const float farPlane, glm::mat4 view);
		std::vector<glm::mat4> GetLightSpaceMatrices(Camera& camera);

		std::vector<float> GetShadowCascadeLevels() const { return m_ShadowCascadeLevels; };
		void SetShadowCascadeLevels(const float cameraFarPlane);
		
		void GenerateCascadedShadowMap(Scene& scene);
		void BindShadowMap(uint32_t slot) const;

		glm::mat4 GetVoxelLightSpaceMatrix(Scene& scene);
		void GenerateVoxelShadowMap(Scene& scene);
		void GenerateVoxelShadowMap(Scene& scene, glm::mat4 lightSpaceMatrix);
		void BindVoxelShadowMap(uint32_t slot) const;

		void UpdateShadowCascades(Camera& camera, const float nearPlane, const float farPlane);
		GlobalShadow CalculateShadowSpaceMatrix(Camera& camera);
		BoundingSphere CalculateFrustumBoundingSphere(Camera& camera, const float nearPlane, const float farPlane);
		bool CascadeNeedsUpdate(glm::mat4 shadowView, uint32_t cascadeNumber, glm::vec3 newCenter, glm::vec3 oldCenter, float radius, glm::vec3& offset);

	private:
		std::string m_Name;
		glm::vec3 m_Direction;
		glm::vec3 m_Color;
		float m_Distance = 100.0f;

		float m_AmbientIntensity;
		float m_DiffuseIntensity;
		float m_SpecularIntensity;

		// Shadow mapping
		bool m_ShadowMapEnabled = true;
		std::shared_ptr<Framebuffer> m_ShadowMapFramebuffer;
		std::shared_ptr<UniformBuffer> m_LightMatricesUniformBuffer;
		uint32_t m_ShadowMapWidth = 4096;
		uint32_t m_ShadowMapHeight = 4096;
		std::vector<float> m_ShadowCascadeLevels;
		std::shared_ptr<Shader> m_ShadowMapShader;
		std::vector<glm::vec3> m_CascadeBoundCenters;

		GlobalShadow m_Global;

		std::shared_ptr<Framebuffer> m_VoxelShadowMapFramebuffer;
		std::shared_ptr<Shader> m_VoxelShadowMapShader;

		float debugDrawSize = 0.2f;
		bool drawDebugEnabled = true;
		float debugDistance = 20.0f;
	};
}