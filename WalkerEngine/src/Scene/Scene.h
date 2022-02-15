#pragma once

#include <entt/entt.hpp>
#include "Camera.h"
#include "Skybox.h"
#include "Renderer/Shader.h"

namespace Walker {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnUpdate(float timestep);
		void EntityDebugPanel();

		std::shared_ptr<Camera> GetCamera() { return m_ActiveCamera; }

		// Temp
		void DrawMeshes(std::shared_ptr<Shader> shader);

	private:
		entt::registry m_Registry;
		std::shared_ptr<Skybox> m_Skybox;
		std::shared_ptr<Camera> m_ActiveCamera;

		friend class Entity;
	};
}