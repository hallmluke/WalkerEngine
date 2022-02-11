#include "walkerpch.h"
#include "Model.h"

#include "Math/Transform.h"
#include "Renderer/Material.h"
//#include <assimp/pbrmaterial.h>

namespace Walker {

    Model::Model(const std::string name, std::string const& path, glm::mat4 initialTransform)
    {
        LoadModel(path);
        m_BaseTransform = initialTransform;
        SetGlobalTransformOnNodes();
    }

    void Model::Draw(std::shared_ptr<Shader> shader)
    {
        m_RootNode->Draw(shader);
    }

    void Model::ControlWindow()
    {
        /*if (ImGui::Begin(name.c_str())) {
            ImGui::Columns(2, nullptr, true);

            rootNode->ShowTree(selectedNode);

            ImGui::NextColumn();
            ImGui::Text("Transform");

            if (selectedNode) {
                ImGui::Text("Orientation");
                if (ImGui::SliderFloat("Roll", &selectedNode->appliedTransform.m_eulerRotation.x, -180.0f, 180.0f)) {
                    selectedNode->ApplyTransform();
                }

                if (ImGui::SliderFloat("Pitch", &selectedNode->appliedTransform.m_eulerRotation.y, -180.0f, 180.0f)) {
                    selectedNode->ApplyTransform();
                }

                if (ImGui::SliderFloat("Yaw", &selectedNode->appliedTransform.m_eulerRotation.z, -180.0f, 180.0f)) {
                    selectedNode->ApplyTransform();
                }
                ImGui::Text("Position");

                if (ImGui::SliderFloat("X", &selectedNode->appliedTransform.m_pos.x, -20.0f, 20.0f)) {
                    selectedNode->ApplyTransform();
                }
                if (ImGui::SliderFloat("Y", &selectedNode->appliedTransform.m_pos.y, -20.0f, 20.0f)) {
                    selectedNode->ApplyTransform();
                }
                if (ImGui::SliderFloat("Z", &selectedNode->appliedTransform.m_pos.z, -20.0f, 20.0f)) {
                    selectedNode->ApplyTransform();
                }

                ImGui::Text("Scale");
                if (ImGui::SliderFloat("Scale X", &selectedNode->appliedTransform.m_scale.x, 0.01f, 20.0f)) {
                    selectedNode->ApplyTransform();
                }
                if (ImGui::SliderFloat("Scale Y", &selectedNode->appliedTransform.m_scale.y, 0.01f, 20.0f)) {
                    selectedNode->ApplyTransform();
                }
                if (ImGui::SliderFloat("Scale Z", &selectedNode->appliedTransform.m_scale.z, 0.01f, 20.0f)) {
                    selectedNode->ApplyTransform();
                }
            }
        }
        ImGui::End();*/
    }

    void Model::SetGlobalTransformOnNodes()
    {
        m_RootNode->SetRootTransform(m_BaseTransform);
        m_RootNode->UpdateGlobalTransform();
    }

    void Model::LoadModel(std::string const& path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            W_CORE_ERROR("Error occurred importing model: {0}", importer.GetErrorString());
            return;
        }
        

        std::string directory = path.substr(0, path.find_last_of('/') + 1);


        m_Materials = ProcessMaterials(scene, directory);
        
        m_RootNode = ProcessNode(scene->mRootNode, scene);
        W_CORE_INFO("Completed importing model");
    }

    std::unique_ptr<ModelNode> Model::ProcessNode(aiNode* node, const aiScene* scene)
    {
        std::vector<std::unique_ptr<Mesh>> curMeshPtrs;
        curMeshPtrs.reserve(node->mNumMeshes);
        
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            
            curMeshPtrs.push_back(std::move(ProcessMesh(mesh, scene)));
        }

        Transform transform;
        transform.aiMatrix4x4ToGlm(node->mTransformation);

        std::unique_ptr<ModelNode> pNode = std::make_unique<ModelNode>(node->mName.C_Str(), std::move(curMeshPtrs), transform.m_transform);
        
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            pNode->AddChild(ProcessNode(node->mChildren[i], scene));
        }

        return pNode;
    }

    std::unique_ptr<Mesh> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;


        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector;
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }
            
            if (mesh->mTextureCoords[0]) 
            {
                glm::vec2 vec;
                
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;

                // tangent
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;
                // bitangent
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }
        
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
        
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        // return a mesh object created from the extracted mesh data
        return std::make_unique<Mesh>(mesh->mName.C_Str(), vertices, indices, m_Materials[mesh->mMaterialIndex]);
    }

    std::vector<std::shared_ptr<Material>> Model::ProcessMaterials(const aiScene* scene, const std::string directory)
    {
        std::vector<std::shared_ptr<Material>> materials = std::vector<std::shared_ptr<Material>>();
        for (uint32_t i = 0; i < scene->mNumMaterials; i++) {
            materials.push_back(std::make_shared<Material>(scene->mMaterials[i], directory));
        }

        return materials;
    }
}