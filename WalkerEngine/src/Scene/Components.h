#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Mesh.h"
#include "PointLight.h"

namespace Walker {

	class Entity;

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) {}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct RelationshipComponent {
		std::size_t Children{};
		Entity First;
		Entity Previous;
		Entity Next;
		Entity Parent;
	};

	// TODO: Not have these components be pointers?
	struct MeshComponent
	{
		std::shared_ptr<Mesh> MeshPtr;

		MeshComponent() = default;
		MeshComponent(std::shared_ptr<Mesh> mesh) : MeshPtr(mesh) {}
	};

	struct PointLightComponent
	{
		std::shared_ptr<PointLight> PointLightPtr;

		PointLightComponent(std::shared_ptr<PointLight> pointLight) : PointLightPtr(pointLight) {}
	};
}