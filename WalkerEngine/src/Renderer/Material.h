#pragma once

#include <glm/glm.hpp>
#include <assimp/material.h>
#include <assimp/pbrmaterial.h>
//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>
#include "Renderer/Texture.h"
#include "Core/UUID.h"

namespace Walker {

	enum class MaterialAlphaMode {
		ALPHAMODE_OPAQUE,
		ALPHAMODE_MASK,
		ALPHAMODE_BLEND
	};

	class Material {
	public:
		Material();
		Material(UUID uuid);
		Material(aiMaterial* mat, std::string textureDirectory);

		UUID GetUUID() const { return m_UUID; }
		std::string GetName() const { return m_Name; }

		
		glm::vec3& GetBaseColor() { return m_BaseColor; }
		// TODO: Textures should be dynamic, based on shader uniforms
		std::shared_ptr<Texture> GetAlbedo() const { return m_Albedo; }
		std::string GetAlbedoPath() const { return m_AlbedoPath; }

		std::shared_ptr<Texture> GetNormal() const { return m_Normal; }
		std::string GetNormalPath() const { return m_NormalPath; }

		std::shared_ptr<Texture> GetMetallicRoughness() const { return m_MetallicRoughness; }
		std::string GetMetallicRoughnessPath() const { return m_MetallicRoughnessPath; }

		void LoadTextures();
		
		// Temporary
		void BindTextures();
	private:
		UUID m_UUID;
		std::string m_Name;

		std::shared_ptr<Texture> m_Albedo;
		std::string m_AlbedoPath;

		std::shared_ptr<Texture> m_Normal;
		std::string m_NormalPath;

		std::shared_ptr<Texture> m_MetallicRoughness;
		std::string m_MetallicRoughnessPath;

		glm::vec3 m_BaseColor;
		bool m_TwoSided;
		// TODO: Use enums for alpha mode
		std::string m_AlphaMode;
		float m_AlphaCutoff;

		bool m_UseAlbedo = true;
		bool m_UseNormal = true;
		bool m_UseMetallicRoughness = true;

		friend class SceneSerializer;
	};

	class MaterialLibrary
	{
	public:
		void Add(const UUID& uuid, const std::shared_ptr<Material>& material);
		void Add(const std::shared_ptr<Material>& material);

		std::shared_ptr<Material> Get(const UUID& uuid);
		std::shared_ptr<Material> Get(const std::string& name);
		std::vector<std::shared_ptr<Material>> GetMaterials();

		bool Exists(const std::string& name) const;

	private:
		std::unordered_map<UUID, std::shared_ptr<Material>> m_Materials;
		//uint32_t m_MaterialNumber = 0;
	};
}