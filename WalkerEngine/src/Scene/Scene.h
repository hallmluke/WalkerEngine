#pragma once 

#include <entt.hpp>
#include "Camera.h"
#include "Skybox.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "GIProbeComponent.h"
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
		void OnViewportResize(uint32_t width, uint32_t height);

		std::shared_ptr<Camera> GetCamera() { return m_ActiveCamera; }

		std::vector<std::shared_ptr<PointLight>> GetPointLights(std::vector<glm::vec3>& positions);
		std::shared_ptr<DirectionalLight> GetDirectionalLight() const { return m_DirectionalLight; }
		std::vector<std::shared_ptr<GIProbe>> GetGIProbes(std::vector<glm::vec3>& positions, std::vector<glm::vec3>& scales);
		
		void AddMaterial(std::shared_ptr<Material> material);
		std::shared_ptr<Material> GetMaterial(UUID uuid);

		// Temp
		void Voxelize(std::shared_ptr<Shader> shader, glm::mat4 voxelProjection, glm::mat4 voxelView);

		void DrawMeshes(std::shared_ptr<Shader> shader);
		void DrawEditor(std::shared_ptr<Shader> shader, std::shared_ptr<Shader> outlineShader);
		void SetSelectedEntity(uint32_t entity) { m_SelectedEntityId = entity; }

	private:
		entt::registry m_Registry;
		std::shared_ptr<DirectionalLight> m_DirectionalLight;
		std::shared_ptr<Skybox> m_Skybox;
		std::shared_ptr<Camera> m_ActiveCamera;

		// TODO: This is only used in editor, figure out way to remove this from runtime
		uint32_t m_SelectedEntityId = 0;

		MaterialLibrary m_MaterialLibrary;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class MaterialPanel;
		friend class SceneSerializer;
	};
}