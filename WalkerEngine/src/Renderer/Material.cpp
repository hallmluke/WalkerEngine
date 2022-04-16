#include "walkerpch.h"
#include "Material.h"

namespace Walker {

    Material::Material()
    {
        m_Name = "Material";
        m_AlphaMode = "OPAQUE";
    }

	Material::Material(UUID uuid) : m_UUID(uuid)
	{
        m_Name = "Material";
        m_AlphaMode = "OPAQUE";
	}

	Material::Material(aiMaterial* mat, std::string textureDirectory)
	{
        aiString name;
        if (AI_SUCCESS == mat->Get(AI_MATKEY_NAME, &name, NULL)) {
            m_Name = std::string(name.C_Str());
        }
        else {
            W_CORE_WARN("Failed to retrieve name for material");
            m_Name = "Material";
        }
        
        
        aiString fileBaseColor, fileMetallicRoughness, fileNormal;
        if (AI_SUCCESS == mat->GetTexture(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_BASE_COLOR_TEXTURE, &fileBaseColor)) {
            m_AlbedoPath = textureDirectory + fileBaseColor.C_Str();
        }
        else {
            W_CORE_WARN("Falied to retrieve albedo texture for material '{0}'", m_Name);
            m_UseAlbedo = false;
        }

        if (AI_SUCCESS == mat->GetTexture(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLICROUGHNESS_TEXTURE, &fileMetallicRoughness)) {
            m_MetallicRoughnessPath = textureDirectory + fileMetallicRoughness.C_Str();
        }
        else {
            W_CORE_WARN("Falied to retrieve metallic/roughness texture for material '{0}'", m_Name);
            m_UseMetallicRoughness = false;
        }

        if (AI_SUCCESS == mat->GetTexture(aiTextureType_NORMALS, 0, &fileNormal)) {
            m_NormalPath = textureDirectory + fileNormal.C_Str();
        }
        else {
            W_CORE_WARN("Falied to retrieve normal texture for material '{0}'", m_Name);
            m_UseNormal = false;
        }

        LoadTextures();

        bool twoSided = false;
        if (AI_SUCCESS == mat->Get(AI_MATKEY_TWOSIDED, &twoSided, NULL)) {
            m_TwoSided = twoSided;
        }

        aiString alphaMode;
        if (AI_SUCCESS == mat->Get(AI_MATKEY_GLTF_ALPHAMODE, &alphaMode, NULL)) {
            m_AlphaMode = std::string(alphaMode.C_Str());
        }
        else {
            m_AlphaMode = "OPAQUE";
        }

        float alphaCutoff = 0.0;
        if (AI_SUCCESS == mat->Get(AI_MATKEY_GLTF_ALPHACUTOFF, &alphaCutoff, NULL)) {
            m_AlphaCutoff = alphaCutoff;
        }
	}

    void Material::LoadTextures()
    {
        if (!m_AlbedoPath.empty()) {
            m_Albedo = Texture2D::Create(m_AlbedoPath);
        }
        else {
            m_UseAlbedo = false;
        }

        if (!m_NormalPath.empty()) {
            m_Normal = Texture2D::Create(m_NormalPath);
        }
        else {
            m_UseNormal = false;
        }

        if (!m_MetallicRoughnessPath.empty()) {
            m_MetallicRoughness = Texture2D::Create(m_MetallicRoughnessPath);
        }
        else {
            m_UseMetallicRoughness = false;
        }
    }

    void Material::BindTextures()
    {
        if (m_UseAlbedo) {
            m_Albedo->Bind(0);
        }
        if (m_UseMetallicRoughness) {
            m_MetallicRoughness->Bind(1);
        }
        if (m_UseNormal) {
            m_Normal->Bind(2);
        }
    }

	void MaterialLibrary::Add(const UUID& uuid, const std::shared_ptr<Material>& material)
	{
        m_Materials[uuid] = material;
	}

    void MaterialLibrary::Add(const std::shared_ptr<Material>& material)
    {
        Add(material->GetUUID(), material);
    }

    std::shared_ptr<Material> MaterialLibrary::Get(const UUID& uuid)
    {
        return m_Materials[uuid];
    }

    std::shared_ptr<Material> MaterialLibrary::Get(const std::string& name)
    {
        for (auto& mat : m_Materials) {
            if (mat.second->GetName() == name) {
                return mat.second;
            }
        }

        return nullptr;
    }

    std::vector<std::shared_ptr<Material>> MaterialLibrary::GetMaterials()
    {
        std::vector<std::shared_ptr<Material>> materials;
        for(auto& mat : m_Materials) {
            materials.push_back(mat.second);
        }
        return materials;
    }

    bool MaterialLibrary::Exists(const std::string& name) const
    {
        return false;
        // TODO: Real implementation? Is this necessary?
        //return m_Materials.find(name) != m_Materials.end();
    }
}