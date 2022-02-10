#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Renderer/Shader.h"
#include "ModelNode.h"

//unsigned int TextureFromFile(const char* path, const string& directory, bool& transparency, bool gamma = false);

namespace Walker {
    class Model
    {
    public:
        // constructor, expects a filepath to a 3D model.
        Model(const std::string name, std::string const& path, glm::mat4 initialTransform);

        // draws the model, and thus all its meshes
        void Draw(std::shared_ptr<Shader> shader);

        void ControlWindow();
        void SetGlobalTransformOnNodes();

    private:
        std::string m_Name;
        std::unique_ptr<ModelNode> m_RootNode;
        glm::mat4 m_BaseTransform;
        ModelNode* m_SelectedNode;
        std::vector<std::shared_ptr<Material>> m_Materials;

        void LoadModel(std::string const& path);

        // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
        std::unique_ptr<ModelNode> ProcessNode(aiNode* node, const aiScene* scene);

        std::unique_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<std::shared_ptr<Material>> ProcessMaterials(const aiScene* scene, const std::string directory);

        // checks all material textures of a given type and loads the textures if they're not loaded yet.
        // the required info is returned as a Texture struct.
        //vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
    };
}