#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Renderer/Shader.h"
#include "ModelNode.h"

namespace Walker {
    class Model
    {
    public:
        Model(const std::string name, std::string const& path, glm::mat4 initialTransform);

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

        std::unique_ptr<ModelNode> ProcessNode(aiNode* node, const aiScene* scene);
        std::unique_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<std::shared_ptr<Material>> ProcessMaterials(const aiScene* scene, const std::string directory);

    };
}