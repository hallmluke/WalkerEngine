#pragma once

#include <glm/glm.hpp>
#include <assimp/material.h>
#include <assimp/pbrmaterial.h>
//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>
#include "Renderer/Texture.h"

namespace Walker {

	enum class MaterialAlphaMode {
		ALPHAMODE_OPAQUE,
		ALPHAMODE_MASK,
		ALPHAMODE_BLEND
	};

	class Material {
	public:
		Material(aiMaterial* mat, std::string textureDirectory);

		std::string GetName() const { return m_Name; }

		
		glm::vec3& GetBaseColor() { return m_BaseColor; }
		// TODO: Textures should be dynamic, based on shader uniforms
		std::shared_ptr<Texture> GetAlbedo() const { return m_Albedo; }
		std::shared_ptr<Texture> GetNormal() const { return m_Normal; }
		std::shared_ptr<Texture> GetMetallicRoughness() const { return m_MetallicRoughness; }
		
		// Temporary
		void BindTextures();
	private:
		std::string m_Name;
		std::shared_ptr<Texture> m_Albedo;
		std::shared_ptr<Texture> m_Normal;
		std::shared_ptr<Texture> m_MetallicRoughness;
		glm::vec3 m_BaseColor;
		bool m_TwoSided;
		// TODO: Use enums for alpha mode
		std::string m_AlphaMode;
		float m_AlphaCutoff;

		bool m_UseAlbedo = true;
		bool m_UseNormal = true;
		bool m_UseMetallicRoughness = true;
	};
}