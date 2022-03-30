#include "walkerpch.h"
#include "DirectionalLight.h"
#include "Renderer/Shader.h"
#include "imgui\imgui.h";
#include <glm/gtx/string_cast.hpp>
#include "Camera.h"
#include "Model.h"

namespace Walker {
	
	DirectionalLight::DirectionalLight(glm::vec3 dir, bool shadowMapEnabled) : m_Direction(dir)
	{
		m_Name = "Directional Light";
		m_AmbientIntensity = 0.1f;
		m_DiffuseIntensity = 8.0f;
		m_SpecularIntensity = 1.0f;
		m_Color = glm::vec3(0.8f);

		m_ShadowMapEnabled = shadowMapEnabled;

		FramebufferSpecification fbSpec;
		fbSpec.Attachments = {
			{ "Depth", FramebufferTextureFormat::DEPTH32F, FramebufferTextureType::FLOAT, FramebufferTextureTarget::TEXTURE_2D_ARRAY }
		}; // TODO: Border for texture
		fbSpec.Width = m_ShadowMapWidth;
		fbSpec.Height = m_ShadowMapHeight;
		fbSpec.Depth = 5; // TODO: Correspond to number of cascades
		fbSpec.Samples = 1;
		m_ShadowMapFramebuffer = Framebuffer::Create(fbSpec);

		m_LightMatricesUniformBuffer = UniformBuffer::Create(sizeof(glm::mat4) * 16, 0);

		SetShadowCascadeLevels(70.0f); // TODO: retrieve from camera
		m_ShadowMapShader = Shader::Create("DepthCascades", "Shaders/depth_shader_cascades.vert", "Shaders/depth_shader.frag", "Shaders/depth_shader_cascades.geom"); // TODO: Use shader library

		FramebufferSpecification voxelFbSpec;
		voxelFbSpec.Attachments = {
			{ "Depth", FramebufferTextureFormat::DEPTH32F, FramebufferTextureType::FLOAT, FramebufferTextureTarget::TEXTURE_2D }
		};
		voxelFbSpec.Width = m_ShadowMapWidth;
		voxelFbSpec.Height = m_ShadowMapHeight;
		voxelFbSpec.Samples = 1;
		m_VoxelShadowMapFramebuffer = Framebuffer::Create(voxelFbSpec);
		m_VoxelShadowMapShader = Shader::Create("Depth", "Shaders/depth_shader.vert", "Shaders/depth_shader.frag");

		m_CascadeBoundCenters.reserve(m_ShadowCascadeLevels.size() + 1);
		for (uint32_t i = 0; i < m_ShadowCascadeLevels.size() + 1; i++) {
			m_CascadeBoundCenters.push_back(glm::vec3(0.0f));
		}
	}

	glm::mat4 DirectionalLight::GetLightSpaceMatrix(Camera& camera, const float nearPlane, const float farPlane)
	{
		std::vector<glm::vec4> corners = camera.GetFrustumCornersWorldSpace(glm::perspective(glm::radians(camera.GetZoom()), camera.GetAspectRatio(), nearPlane, farPlane), camera.GetViewMatrix());

		glm::vec3 center = glm::vec3(0, 0, 0);
		for (const auto& v : corners)
		{
			center += glm::vec3(v);
		}
		center /= corners.size();

		glm::vec3 centerDirection = center - m_Direction;


		float radius = 0.0f;
		for (uint32_t i = 0; i < 8; i++) {
			float distance = glm::length(glm::vec3(corners[i].x, corners[i].y, corners[i].z) - center);
			radius = glm::max(radius, distance);
		}
		//float scaleFactor = (farPlane - nearPlane) / (float) m_ShadowMapWidth;
		float scaleFactor = 1.0f;
		radius = std::ceil(radius * scaleFactor) / scaleFactor;

		glm::vec3 maxExtents = glm::vec3(radius);
		glm::vec3 minExtents = -maxExtents;

		const auto lightView = glm::lookAt(
			center - m_Direction,
			center,
			glm::vec3(0.0f, 1.0f, 0.0f)
		);

		float minX = std::numeric_limits<float>::max();
		float maxX = std::numeric_limits<float>::min();
		float minY = std::numeric_limits<float>::max();
		float maxY = std::numeric_limits<float>::min();
		float minZ = std::numeric_limits<float>::max();
		float maxZ = std::numeric_limits<float>::min();
		for (const auto& v : corners)
		{
			const auto trf = lightView * v;
			minX = std::min(minX, trf.x);
			maxX = std::max(maxX, trf.x);
			minY = std::min(minY, trf.y);
			maxY = std::max(maxY, trf.y);
			minZ = std::min(minZ, trf.z);
			maxZ = std::max(maxZ, trf.z);
		}

		if (minZ < 0)
		{
			minZ *= debugDistance;
		}
		else
		{
			minZ /= debugDistance;
		}
		if (maxZ < 0)
		{
			maxZ /= debugDistance;
		}
		else
		{
			if (maxZ < 0.5f) {
				maxZ = 0.5f;
			}
			//maxZ += 0.1;
			maxZ *= debugDistance;
		}

		const auto lightView2 = glm::lookAt(
			center - m_Direction * -minExtents.z,
			center,
			glm::vec3(0.0f, 1.0f, 0.0f)
		);

		const glm::mat4 lightProjection2 = glm::ortho(minExtents.x, maxExtents.x, minExtents.y, maxExtents.y, 0.0f, maxExtents.z - minExtents.z);

		const glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);

		return lightProjection * lightView;
	}

	glm::mat4 DirectionalLight::GetLightSpaceMatrix(Camera& camera, const float nearPlane, const float farPlane, glm::mat4 view)
	{
		std::vector<glm::vec4> corners = camera.GetFrustumCornersWorldSpace(glm::perspective(glm::radians(camera.GetZoom()), camera.GetAspectRatio(), nearPlane, farPlane), camera.GetViewMatrix());

		glm::vec3 center = glm::vec3(0, 0, 0);
		for (const auto& v : corners)
		{
			center += glm::vec3(v);
		}
		center /= corners.size();

		//glm::vec3 centerDirection = center - m_Direction;


		float radius = 0.0f;
		for (uint32_t i = 0; i < 8; i++) {
			float distance = glm::length(glm::vec3(corners[i].x, corners[i].y, corners[i].z) - center);
			radius = glm::max(radius, distance);
		}
		radius = std::round(radius * 16.0f) / 16.0f;
		//float scaleFactor = (farPlane - nearPlane) / (float) m_ShadowMapWidth;
		//float scaleFactor = 1.0f;
		//radius = std::ceil(radius * scaleFactor) / scaleFactor;

		float f = (radius * 2.0f) / m_ShadowMapWidth;
		glm::vec4 centerViewSpace = view * glm::vec4(center, 1.0f);

		float minX = centerViewSpace.x - radius;
		minX = std::floor(minX / f) * f;

		float minY = centerViewSpace.y - radius;
		minY = std::floor(minY / f) * f;

		float minZ = centerViewSpace.z - radius;
		float maxZ = centerViewSpace.z + radius;

		float viewportExtent = floor((radius * 2.0f) / f) * f;
		float maxX = minX + viewportExtent;
		float maxY = minY + viewportExtent;
		//glm::vec3 maxExtents = glm::vec3(radius);
		//glm::vec3 minExtents = -maxExtents;

		/*const auto lightView = glm::lookAt(
			center - m_Direction,
			center,
			glm::vec3(0.0f, 1.0f, 0.0f)
		);
		*/
		/*const auto lightView2 = glm::lookAt(
			center - m_Direction * -minExtents.z,
			center,
			glm::vec3(0.0f, 1.0f, 0.0f)
		);

		const glm::mat4 lightProjection2 = glm::ortho(minExtents.x, maxExtents.x, minExtents.y, maxExtents.y, 0.0f, maxExtents.z - minExtents.z);*/

		const glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);

		//return lightProjection2 * lightView2;
		return lightProjection * view;
	}

	std::vector<glm::mat4> DirectionalLight::GetLightSpaceMatrices(Camera& camera)
	{
		std::vector<glm::mat4> matrices;
		
		std::vector<glm::vec4> corners = camera.GetFrustumCornersWorldSpace(camera.GetProjectionMatrix(), camera.GetViewMatrix());

		glm::vec3 center = glm::vec3(0, 0, 0);
		for (const auto& v : corners)
		{
			center += glm::vec3(v);
		}
		center /= corners.size();
		const auto lightView = glm::lookAt(
			center - m_Direction,
			center,
			glm::vec3(0.0f, 1.0f, 0.0f)
		);

		for (size_t i = 0; i < m_ShadowCascadeLevels.size() + 1; ++i)
		{
			if (i == 0)
			{
				matrices.push_back(GetLightSpaceMatrix(camera, camera.GetNearPlane(), m_ShadowCascadeLevels[i]));
			}
			else if (i < m_ShadowCascadeLevels.size())
			{
				matrices.push_back(GetLightSpaceMatrix(camera, m_ShadowCascadeLevels[i - 1], m_ShadowCascadeLevels[i]));
			}
			else
			{
				matrices.push_back(GetLightSpaceMatrix(camera, m_ShadowCascadeLevels[i - 1], camera.GetFarPlane()));
			}
		}
		return matrices;
	}

	void DirectionalLight::GenerateCascadedShadowMap(Scene& scene)
	{
		// Upload light space matrices to uniform buffer
		/*std::vector<glm::mat4> lightSpaceMatrices = GetLightSpaceMatrices(*(scene.GetCamera()));
		for (size_t i = 0; i < lightSpaceMatrices.size(); ++i)
		{
			m_LightMatricesUniformBuffer->SetData(&lightSpaceMatrices[i], sizeof(glm::mat4x4), i * sizeof(glm::mat4x4));
		}*/

		UpdateShadowCascades(*(scene.GetCamera()), scene.GetCamera()->GetNearPlane(), scene.GetCamera()->GetFarPlane());

		m_ShadowMapFramebuffer->Bind();
		m_ShadowMapShader->Bind();
		RenderCommand::SetViewport(0, 0, m_ShadowMapWidth, m_ShadowMapHeight);
		RenderCommand::Clear();
		scene.DrawMeshes(m_ShadowMapShader);
	}

	void DirectionalLight::BindShadowMap(uint32_t slot) const
	{
		m_ShadowMapFramebuffer->BindDepthAttachment(slot);
	}

	glm::mat4 DirectionalLight::GetVoxelLightSpaceMatrix(Scene& scene)
	{
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> scales;
		auto probes = scene.GetGIProbes(positions, scales);

		// TODO: This really only works for one probe, either select best one or refactor to work for all
		for (size_t i = 0; i < probes.size(); i++) {

			auto probe = probes[i];
			float minZ = positions[i].z - (scales[i].z / 2);
			float maxZ = positions[i].z + (scales[i].z / 2);
			float minY = positions[i].y - (scales[i].y / 2);
			float maxY = positions[i].y + (scales[i].y / 2);
			float minX = positions[i].x - (scales[i].x / 2);
			float maxX = positions[i].z + (scales[i].x / 2);

			glm::vec3 centerDirection = positions[i] - m_Direction;

			const glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);

			const auto lightView = glm::lookAt(
				positions[i] - m_Direction,
				positions[i],
				glm::vec3(0.0f, 1.0f, 0.0f)
			);

			return lightProjection * lightView;
		}
	}

	void DirectionalLight::GenerateVoxelShadowMap(Scene& scene)
	{
		GenerateVoxelShadowMap(scene, GetVoxelLightSpaceMatrix(scene));
	}

	void DirectionalLight::GenerateVoxelShadowMap(Scene& scene, glm::mat4 lightSpaceMatrix)
	{
		m_VoxelShadowMapFramebuffer->Bind();
		m_VoxelShadowMapShader->Bind();
		RenderCommand::Clear();

		m_VoxelShadowMapShader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);
		//RenderCommand::SetViewport(0, 0, m_ShadowMapWidth, m_ShadowMapHeight);
		scene.DrawMeshes(m_VoxelShadowMapShader);
	}

	void DirectionalLight::BindVoxelShadowMap(uint32_t slot) const
	{
		m_VoxelShadowMapFramebuffer->BindDepthAttachment(slot);
	}

	void DirectionalLight::UpdateShadowCascades(Camera& camera, const float nearPlane, const float farPlane)
	{
		const float minDistance = 0.0f;

		m_Global = CalculateShadowSpaceMatrix(camera);
		const glm::mat3 shadowOffsetMatrix = glm::mat3(glm::transpose(m_Global.ShadowView));

		for (uint32_t cascade = 0; cascade <= m_ShadowCascadeLevels.size(); cascade++) {
			const float nearSplitDistance = nearPlane + (cascade == 0 ? minDistance : m_ShadowCascadeLevels[cascade - 1]);
			const float farSplitDistance = nearPlane + (cascade == m_ShadowCascadeLevels.size() ? farPlane : m_ShadowCascadeLevels[cascade]);

			const BoundingSphere boundingSphere = CalculateFrustumBoundingSphere(camera, nearSplitDistance, farSplitDistance);

			glm::vec3 offset;

			if (CascadeNeedsUpdate(m_Global.ShadowView, cascade, boundingSphere.FrustumCenter, m_CascadeBoundCenters[cascade], boundingSphere.Radius, offset))
			{
				glm::vec3 offsetWorldSpace = shadowOffsetMatrix * offset;
				m_CascadeBoundCenters[cascade] += offsetWorldSpace;
			}

			glm::vec3 cascadeCenterShadowSpace = glm::vec3(m_Global.WorldToShadowSpace * glm::vec4(m_CascadeBoundCenters[cascade], 1.0f));
			float scale = m_Global.Radius / boundingSphere.Radius;

			// TODO store data

			auto cascadeTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(cascadeCenterShadowSpace.x, cascadeCenterShadowSpace.y, 0.0f));
			auto cascadeTranslateInverse = glm::translate(glm::mat4(1.0f), glm::vec3(-cascadeCenterShadowSpace.x, -cascadeCenterShadowSpace.y, 0.0f));
			auto cascadeScale = glm::scale(glm::mat4(1.0), glm::vec3(scale, scale, 1.0f));

			auto lightViewProjectionMatrix = cascadeScale * cascadeTranslateInverse * m_Global.WorldToShadowSpace;

			m_LightMatricesUniformBuffer->SetData(&lightViewProjectionMatrix, sizeof(glm::mat4x4), cascade * sizeof(glm::mat4x4));
			// Set scale?
		}
	}

	GlobalShadow DirectionalLight::CalculateShadowSpaceMatrix(Camera& camera)
	{
		const float nearPlane = camera.GetNearPlane();
		const float farPlane = camera.GetFarPlane();
		const float cascadeTotalRange = farPlane - nearPlane;
		const BoundingSphere shadowBounds = CalculateFrustumBoundingSphere(camera, nearPlane, farPlane);

		const glm::vec3 cameraFrustumCenterWorldSpace = shadowBounds.FrustumCenter;
		const glm::vec3 lightPosition = cameraFrustumCenterWorldSpace + glm::normalize(m_Direction) * m_Distance;

		glm::vec3 upVec = glm::normalize(glm::cross(m_Direction, glm::vec3(0.0f, 1.0f, 0.0f)));

		const float directionBias = 0.0001f;

		if (glm::abs(glm::dot(upVec, m_Direction)) < directionBias) {
			upVec = glm::normalize(glm::cross(m_Direction, glm::vec3(0.0f, 0.0f, -1.0f)));
		}

		const glm::mat4 shadowView = glm::lookAt(cameraFrustumCenterWorldSpace, lightPosition, upVec);
		const glm::mat4 shadowProj = glm::ortho(-shadowBounds.Radius, shadowBounds.Radius, -shadowBounds.Radius, shadowBounds.Radius, -shadowBounds.Radius, shadowBounds.Radius);

		GlobalShadow result;
		result.WorldToShadowSpace = shadowProj * shadowView;
		result.ShadowView = shadowView;
		result.Radius = shadowBounds.Radius;
		return result;
	}

	BoundingSphere DirectionalLight::CalculateFrustumBoundingSphere(Camera& camera, const float nearPlane, const float farPlane)
	{
		std::vector<glm::vec4> corners = camera.GetFrustumCornersWorldSpace(glm::perspective(glm::radians(camera.GetZoom()), camera.GetAspectRatio(), nearPlane, farPlane), camera.GetViewMatrix());

		glm::vec3 center = glm::vec3(0.0f);
		for (const auto& v : corners)
		{
			center += glm::vec3(v);
		}
		center /= corners.size();

		float radius = 0.0f;
		for (uint32_t i = 0; i < 8; i++) {
			float distance = glm::length(glm::vec3(corners[i].x, corners[i].y, corners[i].z) - center);
			radius = glm::max(radius, distance);
		}
		radius = std::round(radius * 16.0f) / 16.0f;

		BoundingSphere result;
		result.FrustumCenter = center;
		result.Radius = radius;

		return result;
	}

	bool DirectionalLight::CascadeNeedsUpdate(glm::mat4 shadowView, uint32_t cascadeNumber, glm::vec3 newCenter, glm::vec3 oldCenter, float radius, glm::vec3& offset)
	{
		const glm::vec3 oldCenterViewSpace = glm::vec3(shadowView * glm::vec4(oldCenter, 1.0f));
		const glm::vec3 newCenterViewSpace = glm::vec3(shadowView * glm::vec4(newCenter, 1.0f));
		const glm::vec3 centerDiff = newCenterViewSpace - oldCenterViewSpace;

		const float pixelSize = (float)m_ShadowMapWidth / (2.0f * radius);

		const float pixelOffsetX = centerDiff.x * pixelSize;
		const float pixelOffsetY = centerDiff.y * pixelSize;

		const bool needUpdate = glm::abs(pixelOffsetX) > 0.5f || glm::abs(pixelOffsetY) > 0.5f;
		if (needUpdate)
		{
			offset.x = glm::floor(0.5f + pixelOffsetX) / pixelSize;
			offset.y = glm::floor(0.5f + pixelOffsetY) / pixelSize;
			offset.z = centerDiff.z;
		}

		return needUpdate;
	}

	void DirectionalLight::SetShadowCascadeLevels(float cameraFarPlane)
	{
		// TODO: Dynamic number of cascades
		m_ShadowCascadeLevels = std::vector<float> { cameraFarPlane / 50.0f, cameraFarPlane / 25.0f, cameraFarPlane / 10.0f, cameraFarPlane / 3.0f };
	}


}