#pragma once

#include <entt.hpp>
#include "Camera.h"
#include "Skybox.h"
#include "Renderer/Shader.h"
#include "Renderer/Material.h"
#include "Core/UUID.h"

namespace Walker {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnUpdate(float timestep);
		void EntityDebugPanel();

		std::shared_ptr<Camera> GetCamera() { return m_ActiveCamera; }

		std::vector<std::shared_ptr<PointLight>> GetPointLights(std::vector<glm::vec3>& positions);
		std::shared_ptr<DirectionalLight> GetDirectionalLight() const { return m_DirectionalLight; }

		void AddMaterial(std::shared_ptr<Material> material);
		std::shared_ptr<Material> GetMaterial(UUID uuid);

		// Temp
		void DrawMeshes(std::shared_ptr<Shader> shader);
		void DrawEditor(std::shared_ptr<Shader> shader);

	private:
		entt::registry m_Registry;
		std::shared_ptr<DirectionalLight> m_DirectionalLight;
		std::shared_ptr<Skybox> m_Skybox;
		std::shared_ptr<Camera> m_ActiveCamera;

		MaterialLibrary m_MaterialLibrary;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class MaterialPanel;
		friend class SceneSerializer;
	};
}