#include "walkerpch.h"
#include "Material.h"

namespace Walker {

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
            m_Albedo = Texture2D::Create(textureDirectory + fileBaseColor.C_Str());
        }
        else {
            W_CORE_WARN("Falied to retrieve albedo texture for material '{0}'", m_Name);
            m_UseAlbedo = false;
        }


        if (AI_SUCCESS == mat->GetTexture(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLICROUGHNESS_TEXTURE, &fileMetallicRoughness)) {
            m_MetallicRoughness = Texture2D::Create(textureDirectory + fileMetallicRoughness.C_Str());
        }
        else {
            W_CORE_WARN("Falied to retrieve metallic/roughness texture for material '{0}'", m_Name);
            m_UseMetallicRoughness = false;
        }

        if (AI_SUCCESS == mat->GetTexture(aiTextureType_NORMALS, 0, &fileNormal)) {
            m_Normal = Texture2D::Create(textureDirectory + fileNormal.C_Str());
        }
        else {
            W_CORE_WARN("Falied to retrieve normal texture for material '{0}'", m_Name);
            m_UseNormal = false;
        }

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


        
		/*vector<Texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }
            if (!skip)
            {   // if texture hasn't been loaded already, load it
                Texture texture;
                bool transparency = false;
                texture.id = TextureFromFile(str.C_Str(), this->directory, transparency, gammaCorrection);
                texture.type = typeName;
                texture.path = str.C_Str();
                texture.transparency = transparency;
                textures.push_back(texture);
                textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
            }
        }
        return textures;*/
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
}