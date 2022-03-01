#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Renderer/Shader.h"
#include "ModelNode.h"
#include "Scene.h"
#include "Entity.h"

namespace Walker {
    class Model
    {
    public:
        Model(const std::string name, std::string const& path, glm::mat4 initialTransform);
        Model(const std::string name, std::string const& path, Scene* walkerScene);

        void Draw(std::shared_ptr<Shader> shader);

        void SetGlobalTransformOnNodes();

    private:
        std::string m_Name;
        std::unique_ptr<ModelNode> m_RootNode;
        glm::mat4 m_BaseTransform;
        ModelNode* m_SelectedNode;
        std::vector<std::shared_ptr<Material>> m_Materials;

        void LoadModel(std::string const& path);
        void LoadModelECS(std::string const& path, Scene* walkerScene);

        std::unique_ptr<ModelNode> ProcessNode(aiNode* node, const aiScene* scene);
        Entity ProcessNodeECS(aiNode* node, const aiScene* scene, Scene* walkerScene);
        std::shared_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<std::shared_ptr<Material>> ProcessMaterials(const aiScene* scene, Scene* walkerScene, const std::string directory);

    };
}