#include "walkerpch.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h" 
#include "IDComponent.h"

#include "imgui/imgui.h"
#include "glm/gtx/string_cast.hpp"


namespace Walker {

	Scene::Scene()
	{
		// TODO: Remove hardcoding size
		m_ActiveCamera = std::make_shared<Camera>(glm::vec3(1.0f), 1600, 900);
		m_DirectionalLight = std::make_shared<DirectionalLight>(glm::vec3(0.0f, -1.0f, -0.1f));
		//m_Skybox = std::make_shared<Skybox>("Skybox/default");

		/*Entity defaultPointLight = CreateEntity("Default Pointlight");
		auto& pointLightComponent = defaultPointLight.AddComponent<PointLightComponent>();
		auto& pointLightPosition = defaultPointLight.GetComponent<TransformComponent>();

		pointLightPosition.Translation = glm::vec3(0.0, 1.0, 0.0);*/
		
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntityWithUUID(UUID(), name);
	}

	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		auto& relationships = entity.AddComponent<RelationshipComponent>();
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>();

		W_CORE_TRACE("Deleting entity {0}", tag.Tag);
		auto& rel = entity.GetComponent<RelationshipComponent>();

		// Kill all children
		if (rel.Children > 0) {
			Entity child = rel.First;
			Entity next;

			while (child) {
				next = child.GetComponent<RelationshipComponent>().Next;
				DestroyEntity(child);
				child = next;
			}
		}

		if (rel.Previous && m_Registry.valid(rel.Previous)) {
			auto& prevRel = rel.Previous.GetComponent<RelationshipComponent>();
			prevRel.Next = rel.Next;
		}

		if (rel.Next && m_Registry.valid(rel.Next)) {
			auto& nextRel = rel.Next.GetComponent<RelationshipComponent>();
			nextRel.Previous = rel.Previous;
		}

		if (rel.Parent) {
			auto& parentRel = rel.Parent.GetComponent<RelationshipComponent>();
			parentRel.Children--;
			if (parentRel.First == entity) {
				parentRel.First = rel.Next;
			}
		}

		m_Registry.destroy(entity);
	}

	void Scene::OnUpdate(float timestep)
	{
		m_ActiveCamera->OnUpdate(timestep);
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ActiveCamera->ResizeViewport(width, height);
	}

	std::vector<std::shared_ptr<PointLight>> Scene::GetPointLights(std::vector<glm::vec3>& positions)
	{
		std::vector<std::shared_ptr<PointLight>> lights;
		std::vector<glm::vec3> pos;
		auto view = m_Registry.view<PointLightComponent, TransformComponent>();

		for (auto entity : view) {
			auto [pointLightComp, transformComp] = view.get<PointLightComponent, TransformComponent>(entity);
			lights.push_back(pointLightComp.PointLightPtr);
			pos.push_back(transformComp.Translation);
		}

		positions = pos;

		return lights;
	}

	void Scene::AddMaterial(std::shared_ptr<Material> material)
	{
		m_MaterialLibrary.Add(material);
	}

	std::shared_ptr<Material> Scene::GetMaterial(UUID uuid)
	{
		return m_MaterialLibrary.Get(uuid);
	}

	void Scene::Voxelize(std::shared_ptr<Shader> shader)
	{
		glm::mat4 voxelProjection = glm::ortho(-128.0f, 128.0f, -128.0f, 128.0f, -128.0f, 128.0f);
		glm::mat4 voxelView = glm::lookAt(glm::vec3(0.0f, 0.0f, 64.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		shader->SetMat4("view", voxelView);
		shader->SetMat4("projection", voxelProjection);
		auto view = m_Registry.view<TransformComponent, RelationshipComponent, MeshComponent>();
		for (auto entity : view)
		{
			auto [transform, relationship, mesh] = view.get<TransformComponent, RelationshipComponent, MeshComponent>(entity);

			glm::mat4 globalTransform = transform.GetTransform();

			// TODO: Persistently track global transform instead of calculating before drawing
			Entity parent = relationship.Parent;
			while (parent) {
				glm::mat4 parentTransform = parent.GetComponent<TransformComponent>().GetTransform();
				globalTransform = parentTransform * globalTransform;
				parent = parent.GetComponent<RelationshipComponent>().Parent;
				//globalTransform = globalTransform * parentTransform;
			}

			globalTransform = glm::scale(globalTransform, glm::vec3(3.0));
			mesh.MeshPtr->Draw(shader, globalTransform);
		}


		/*voxelProjection = glm::ortho(-128.0f, 128.0f, -128.0f, 128.0f, -128.0f, 128.0f);
		voxelView = glm::lookAt(glm::vec3(0.0f, 64.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		shader->SetMat4("view", voxelView);
		shader->SetMat4("projection", voxelProjection);
		view = m_Registry.view<TransformComponent, RelationshipComponent, MeshComponent>();
		for (auto entity : view)
		{
			auto [transform, relationship, mesh] = view.get<TransformComponent, RelationshipComponent, MeshComponent>(entity);

			glm::mat4 globalTransform = transform.GetTransform();

			// TODO: Persistently track global transform instead of calculating before drawing
			Entity parent = relationship.Parent;
			while (parent) {
				glm::mat4 parentTransform = parent.GetComponent<TransformComponent>().GetTransform();
				globalTransform = parentTransform * globalTransform;
				parent = parent.GetComponent<RelationshipComponent>().Parent;
				//globalTransform = globalTransform * parentTransform;
			}

			globalTransform = glm::scale(globalTransform, glm::vec3(3.0));

			mesh.MeshPtr->Draw(shader, globalTransform);
		}*/
		
		/*glm::mat4 voxelProjection = glm::ortho(-128.0f, 128.0f, -128.0f, 128.0f, -128.0f, 128.0f);
		glm::mat4 voxelView = glm::lookAt(glm::vec3(64.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		shader->SetMat4("view", voxelView);
		shader->SetMat4("projection", voxelProjection);
		auto view = m_Registry.view<TransformComponent, RelationshipComponent, MeshComponent>();
		for (auto entity : view)
		{
			auto [transform, relationship, mesh] = view.get<TransformComponent, RelationshipComponent, MeshComponent>(entity);

			glm::mat4 globalTransform = transform.GetTransform();

			// TODO: Persistently track global transform instead of calculating before drawing
			Entity parent = relationship.Parent;
			while (parent) {
				glm::mat4 parentTransform = parent.GetComponent<TransformComponent>().GetTransform();
				globalTransform = parentTransform * globalTransform;
				parent = parent.GetComponent<RelationshipComponent>().Parent;
				//globalTransform = globalTransform * parentTransform;
			}

			globalTransform = glm::scale(globalTransform, glm::vec3(3.0));

			mesh.MeshPtr->Draw(shader, globalTransform);
		}*/
	}

	void Scene::DrawMeshes(std::shared_ptr<Shader> shader)
	{
		shader->SetMat4("view", m_ActiveCamera->GetViewMatrix());
		shader->SetMat4("projection", m_ActiveCamera->GetProjectionMatrix());
		auto view = m_Registry.view<TransformComponent, RelationshipComponent, MeshComponent>();
		for (auto entity : view)
		{
			auto [transform, relationship, mesh] = view.get<TransformComponent, RelationshipComponent, MeshComponent>(entity);

			glm::mat4 globalTransform = transform.GetTransform();

			// TODO: Persistently track global transform instead of calculating before drawing
			Entity parent = relationship.Parent;
			while (parent) {
				glm::mat4 parentTransform = parent.GetComponent<TransformComponent>().GetTransform();
				globalTransform = parentTransform * globalTransform;
				parent = parent.GetComponent<RelationshipComponent>().Parent;
				//globalTransform = globalTransform * parentTransform;
			}

			globalTransform = glm::scale(globalTransform, glm::vec3(3.0));

			mesh.MeshPtr->Draw(shader, globalTransform);
		}
	}
	void Scene::DrawEditor(std::shared_ptr<Shader> shader, std::shared_ptr<Shader> outlineShader)
	{
		shader->Bind();
		shader->SetMat4("view", m_ActiveCamera->GetViewMatrix());
		shader->SetMat4("projection", m_ActiveCamera->GetProjectionMatrix());
		auto view = m_Registry.view<TransformComponent, RelationshipComponent, MeshComponent>();

		
		Entity m_SelectedEntity = Entity{ (entt::entity) m_SelectedEntityId, this };

		RenderCommand::SetStencilMask(0x00);

		for (auto entity : view)
		{
			shader->SetInt("entityId", (int32_t) entity);
			auto [transform, relationship, mesh] = view.get<TransformComponent, RelationshipComponent, MeshComponent>(entity);

			glm::mat4 globalTransform = transform.GetTransform();

			// TODO: Persistently track global transform instead of calculating before drawing
			Entity parent = relationship.Parent;
			while (parent) {
				glm::mat4 parentTransform = parent.GetComponent<TransformComponent>().GetTransform();
				globalTransform = parentTransform * globalTransform;
				parent = parent.GetComponent<RelationshipComponent>().Parent;
				//globalTransform = globalTransform * parentTransform;
			}

			if (m_SelectedEntity != entity) {
				mesh.MeshPtr->Draw(shader, globalTransform);
			}
		}

		if (m_SelectedEntity.ValidEntity() && m_SelectedEntity.HasComponent<MeshComponent>()) {

			RenderCommand::SetStencilFunction(RendererAPI::StencilFunction::ALWAYS, 1, 0xFF);
			RenderCommand::SetStencilMask(0xFF);

			auto transform = m_SelectedEntity.GetComponent<TransformComponent>();
			auto relationship = m_SelectedEntity.GetComponent<RelationshipComponent>();
			auto mesh = m_SelectedEntity.GetComponent<MeshComponent>();

			shader->SetInt("entityId", (int32_t)m_SelectedEntity);

			glm::mat4 globalTransform = transform.GetTransform();

			// TODO: Persistently track global transform instead of calculating before drawing
			Entity parent = relationship.Parent;
			while (parent) {
				glm::mat4 parentTransform = parent.GetComponent<TransformComponent>().GetTransform();
				globalTransform = parentTransform * globalTransform;
				parent = parent.GetComponent<RelationshipComponent>().Parent;
				//globalTransform = globalTransform * parentTransform;
			}

			mesh.MeshPtr->Draw(shader, globalTransform);

			outlineShader->Bind();
			outlineShader->SetMat4("view", m_ActiveCamera->GetViewMatrix());
			outlineShader->SetMat4("projection", m_ActiveCamera->GetProjectionMatrix());
			outlineShader->SetInt("entityId", m_SelectedEntityId);

			RenderCommand::SetStencilFunction(RendererAPI::StencilFunction::NOTEQUAL, 1, 0xFF);
			RenderCommand::SetStencilMask(0x00);
			//RenderCommand::DisableDepthTest();
			RenderCommand::DisableBackfaceCulling();

			//globalTransform = glm::scale(globalTransform, glm::vec3(1.01, 1.01f, 1.01f));
			mesh.MeshPtr->Draw(outlineShader, globalTransform);

			RenderCommand::SetStencilFunction(RendererAPI::StencilFunction::ALWAYS, 0, 0xFF);
			//RenderCommand::EnableDepthTest();
			RenderCommand::EnableBackfaceCulling();
		}
	}
}