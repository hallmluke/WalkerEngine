#include "walkerpch.h"
#include "SceneSerializer.h"

#include "Components.h"
#include "Mesh.h"
#include "Renderer/Material.h"

#include <fstream>
#include <filesystem>


namespace YAML {

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

}

namespace Walker {

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(const std::shared_ptr<Scene>& scene)
		: m_Scene(scene)
	{
	}

	static void SerializeMaterial(YAML::Emitter& out, Material material) {
		out << YAML::BeginMap;
		out << YAML::Key << "Material" << YAML::Value << material.GetUUID();

		out << YAML::Key << "m_Name" << YAML::Value << material.GetName();
		out << YAML::Key << "m_AlbedoPath" << YAML::Value << material.GetAlbedoPath();
		out << YAML::Key << "m_NormalPath" << YAML::Value << material.GetNormalPath();
		out << YAML::Key << "m_MetallicRoughnessPath" << YAML::Value << material.GetMetallicRoughnessPath();

		out << YAML::EndMap;
	}

	static void SerializeMeshBinary(const std::string& filepath, std::shared_ptr<Mesh> meshPtr) {

		std::fstream file;
		file.open(filepath, std::ios::app | std::ios::binary);

		for (auto vertex : meshPtr->GetVertices()) {
			file.write(reinterpret_cast<char*>(&vertex), sizeof(vertex));
			//file.write(reinterpret_cast<char*>(&vertex.Position), sizeof(vertex.Position));
			//file.write(reinterpret_cast<char*>())
			/*out << YAML::BeginMap;

			out << YAML::Key << "Position" << YAML::Value << vertex.Position;
			out << YAML::Key << "Normal" << YAML::Value << vertex.Normal;
			out << YAML::Key << "TexCoords" << YAML::Value << vertex.TexCoords;
			out << YAML::Key << "Tangent" << YAML::Value << vertex.Tangent;
			out << YAML::Key << "Bitangent" << YAML::Value << vertex.Bitangent;

			out << YAML::EndMap;*/
		}

		/*out << YAML::EndSeq;

		out << YAML::Key << "m_Indices";
		out << YAML::Flow;
		out << YAML::BeginSeq;*/

		for (auto index : meshPtr->GetIndices()) {
			file.write(reinterpret_cast<char*>(&index), sizeof(index));
			//out << index;
		}
	}

	static void DeserializeMeshBinary(const std::string& filepath, size_t vertexCount, size_t indexCount, std::vector<Vertex>& vertexOut, std::vector<uint32_t>& indexOut) {
		std::ifstream is(filepath, std::ifstream::binary);

		if (is) {
			Vertex vertex{};
			size_t currentVertex = 0;
			
			while (currentVertex < vertexCount && is.read(reinterpret_cast<char*>(&vertex), sizeof(vertex)) ) {
				vertexOut.push_back(vertex);
				currentVertex++;
			}

			uint32_t index{};
			while (is.read(reinterpret_cast<char*>(&index), sizeof(index))) {
				indexOut.push_back(index);
			}

			if (indexOut.size() != indexCount) {
				W_CORE_ERROR("Error deserializing mesh: Index vector size: {0}, indexCount: {1}", indexOut.size(), indexCount);
			}
		}
		else {
			W_CORE_ERROR("Error deserializing mesh: unable to read {0}", filepath);
		}
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity, std::string sceneFilePath)
	{
		out << YAML::BeginMap; // Entity
		out << YAML::Key << "Entity" << YAML::Value << entity.GetUUID();

		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap; // TagComponent

			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap; // TagComponent
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; // TransformComponent

			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

			out << YAML::EndMap; // TransformComponent
		}

		if (entity.HasComponent<PointLightComponent>())
		{
			out << YAML::Key << "PointLightComponent";
			out << YAML::BeginMap;

			auto& pl = entity.GetComponent<PointLightComponent>().PointLightPtr;
			out << YAML::Key << "m_AmbientIntensity" << YAML::Value << pl->GetAmbientIntensity();
			out << YAML::Key << "m_DiffuseIntensity" << YAML::Value << pl->GetDiffuseIntensity();
			out << YAML::Key << "m_SpecularIntensity" << YAML::Value << pl->GetSpecularIntensity();
			out << YAML::Key << "m_ConstantAttenuation" << YAML::Value << pl->GetConstantAttenuation();
			out << YAML::Key << "m_LinearAttenuation" << YAML::Value << pl->GetLinearAttenuation();
			out << YAML::Key << "m_QuadraticAttenuation" << YAML::Value << pl->GetQuadraticAttenuation();

			out << YAML::EndMap;
		}

		if (entity.HasComponent<MeshComponent>())
		{
			out << YAML::Key << "MeshComponent";
			out << YAML::BeginMap;

			auto& mesh = entity.GetComponent<MeshComponent>().MeshPtr;
			std::string file = "mesh" + std::to_string(entity.GetUUID()) + ".wmesh";
			std::filesystem::path scenePath = std::filesystem::path(sceneFilePath);
			std::string filepath = scenePath.parent_path().string() + "/" + file;

			SerializeMeshBinary(filepath, mesh);
			out << YAML::Key << "BinPath" << YAML::Value << filepath;
			out << YAML::Key << "VertexCount" << YAML::Value << mesh->GetVertices().size();
			out << YAML::Key << "IndexCount" << YAML::Value << mesh->GetIndices().size();

			out << YAML::Key << "m_Material" << YAML::Value << mesh->GetMaterial()->GetUUID();

			out << YAML::EndMap;
		}

		if (entity.HasComponent<RelationshipComponent>()) {
			auto& rel = entity.GetComponent<RelationshipComponent>();

			if (rel.Children > 0) {
				out << YAML::Key << "Children";
				out << YAML::BeginSeq;

				Entity& child = rel.First;

				while (child) {
					SerializeEntity(out, child, sceneFilePath);
					child = child.GetComponent<RelationshipComponent>().Next;
				}
			}
		}

		/*if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap; // CameraComponent

			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.Camera;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap; // Camera
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveVerticalFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
			out << YAML::EndMap; // Camera

			out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;

			out << YAML::EndMap; // CameraComponent
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap; // SpriteRendererComponent

			auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;

			out << YAML::EndMap; // SpriteRendererComponent
		}*/

		out << YAML::EndMap; // Entity
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Materials" << YAML::Value << YAML::BeginSeq;
		auto& materials = m_Scene->m_MaterialLibrary.GetMaterials();
		for (auto& mat : materials) {
			SerializeMaterial(out, *mat);
		}
		out << YAML::EndSeq;
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene->m_Registry.each([&](auto entityID)
		{
			Entity entity = { entityID, m_Scene.get() };
			if (!entity)
				return;

			auto& rel = entity.GetComponent<RelationshipComponent>();

			if (!rel.Parent) {
				SerializeEntity(out, entity, filepath);
			}
		});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();

	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
		W_CORE_ERROR("Serialize runtime not implemented");
	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		W_CORE_TRACE("Loading scene: {0}", filepath);
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filepath);
		}
		catch (YAML::ParserException e)
		{
			return false;
		}

		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		W_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		auto materials = data["Materials"];
		if (materials) {
			for (auto mat : materials) {
				uint64_t uuid = mat["Material"].as<uint64_t>();

				std::shared_ptr<Material> material = std::make_shared<Material>(uuid);

				W_CORE_TRACE("Deserializing material with ID = {0}", uuid);

				material->m_Name = mat["m_Name"].as<std::string>();
				material->m_AlbedoPath = mat["m_AlbedoPath"].as<std::string>();
				material->m_NormalPath = mat["m_NormalPath"].as<std::string>();
				material->m_MetallicRoughnessPath = mat["m_MetallicRoughnessPath"].as<std::string>();
				material->LoadTextures();

				m_Scene->AddMaterial(material);
			}
		}

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				DeserializeEntity(entity);
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		W_CORE_ERROR("Deserialize runtime not implemented");
		return false;
	}

	Entity& SceneSerializer::DeserializeEntity(YAML::Node entity)
	{
		uint64_t uuid = entity["Entity"].as<uint64_t>(); // TODO

		std::string name;
		auto tagComponent = entity["TagComponent"];
		if (tagComponent)
			name = tagComponent["Tag"].as<std::string>();

		W_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);

		Entity deserializedEntity = m_Scene->CreateEntityWithUUID(uuid, name);

		auto transformComponent = entity["TransformComponent"];
		if (transformComponent)
		{
			// Entities always have transforms
			auto& tc = deserializedEntity.GetComponent<TransformComponent>();
			tc.Translation = transformComponent["Translation"].as<glm::vec3>();
			tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
			tc.Scale = transformComponent["Scale"].as<glm::vec3>();
		}

		W_CORE_TRACE("Deserialized transform");

		auto meshComponent = entity["MeshComponent"];
		if (meshComponent) {
			std::vector<Vertex> vertices;
			std::vector<uint32_t> indices;

			std::string filepath = meshComponent["BinPath"].as<std::string>();
			size_t vertexCount = meshComponent["VertexCount"].as<size_t>();
			size_t indexCount = meshComponent["IndexCount"].as<size_t>();

			DeserializeMeshBinary(filepath, vertexCount, indexCount, vertices, indices);

			std::shared_ptr<Material> mat = m_Scene->GetMaterial(meshComponent["m_Material"].as<uint64_t>());

			W_CORE_TRACE("Retrieved material for mesh");

			std::shared_ptr<Mesh> meshPtr = std::make_shared<Mesh>("Mesh", vertices, indices, mat);

			deserializedEntity.AddComponent<MeshComponent>(meshPtr);
		}

		W_CORE_TRACE("Deserialized mesh");

		auto pointLightComponent = entity["PointLightComponent"];
		if (pointLightComponent) {

			std::shared_ptr<PointLight> pl = std::make_shared<PointLight>();
			pl->m_AmbientIntensity = pointLightComponent["m_AmbientIntensity"].as<float>();
			W_CORE_TRACE("1");
			pl->m_DiffuseIntensity = pointLightComponent["m_DiffuseIntensity"].as<float>();
			pl->m_SpecularIntensity = pointLightComponent["m_SpecularIntensity"].as<float>();
			pl->m_ConstantAttenuation = pointLightComponent["m_ConstantAttenuation"].as<float>();
			pl->m_LinearAttenuation = pointLightComponent["m_LinearAttenuation"].as<float>();
			pl->m_QuadraticAttenuation = pointLightComponent["m_QuadraticAttenuation"].as<float>();
			W_CORE_TRACE("2");
			auto& plc = deserializedEntity.AddComponent<PointLightComponent>(pl);
		}

		auto children = entity["Children"];
		if (children) {

			auto& rel = deserializedEntity.GetComponent<RelationshipComponent>();
			rel.Children = children.size();

			Entity prev;
			Entity current;

			for (auto& entity : children) {
				current = DeserializeEntity(entity);
				
				if (!rel.First) {
					rel.First = current;
				}

				auto& currentRel = current.GetComponent<RelationshipComponent>();
				currentRel.Parent = deserializedEntity;
				if (prev) {
					auto& previousRel = prev.GetComponent<RelationshipComponent>();
					currentRel.Previous = prev;
					previousRel.Next = current;
				}

				prev = current;
			}
		}

		return deserializedEntity;
	}

}