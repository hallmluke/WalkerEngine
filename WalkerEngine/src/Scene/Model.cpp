#include "walkerpch.h"
#include "Model.h"

#include "Math/Transform.h"
#include "Renderer/Material.h"
#include "Components.h"
//#include <assimp/pbrmaterial.h>
#include <assimp/material.h>

namespace Walker {

    Model::Model(const std::string name, std::string const& path, glm::mat4 initialTransform)
    {
        LoadModel(path);
        m_BaseTransform = initialTransform;
        SetGlobalTransformOnNodes();
    }

    Model::Model(const std::string name, std::string const& path, Scene* walkerScene)
    {
        LoadModelECS(path, walkerScene);
    }

    void Model::Draw(std::shared_ptr<Shader> shader)
    {
        m_RootNode->Draw(shader);
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

    void Model::LoadModelECS(std::string const& path, Scene* walkerScene)
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
        ProcessNodeECS(scene->mRootNode, scene, walkerScene);
    }

    std::unique_ptr<ModelNode> Model::ProcessNode(aiNode* node, const aiScene* scene)
    {
        std::vector<std::shared_ptr<Mesh>> curMeshPtrs;
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

    Entity Model::ProcessNodeECS(aiNode* node, const aiScene* scene, Scene* walkerScene)
    {
        Entity entity = walkerScene->CreateEntity(node->mName.C_Str());
        TransformComponent& transformComponent = entity.GetComponent<TransformComponent>();
        aiVector3D pos;
        aiVector3D rot;
        aiVector3D scale;
        node->mTransformation.Decompose(scale, rot, pos);

        transformComponent.Translation.x = pos.x;
        transformComponent.Translation.y = pos.y;
        transformComponent.Translation.z = pos.z;

        transformComponent.Rotation.x = rot.x;
        transformComponent.Rotation.y = rot.y;
        transformComponent.Rotation.z = rot.z;

        transformComponent.Scale.x = scale.x;
        transformComponent.Scale.y = scale.y;
        transformComponent.Scale.z = scale.z;

        RelationshipComponent& relationshipComponent = entity.GetComponent<RelationshipComponent>();

        Entity previous;

        for (uint32_t i = 0; i < node->mNumMeshes; i++)
        {
            relationshipComponent.Children++;
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            Entity meshChild = walkerScene->CreateEntity(mesh->mName.C_Str());
            MeshComponent& meshComponent = meshChild.AddComponent<MeshComponent>();

            meshComponent.MeshPtr = ProcessMesh(mesh, scene);

            RelationshipComponent& meshRelationshipComponent = meshChild.GetComponent<RelationshipComponent>();
            meshRelationshipComponent.Parent = entity;

            if (previous) {
                meshRelationshipComponent.Previous = previous;
                RelationshipComponent& previousRelationshipComponent = previous.GetComponent<RelationshipComponent>();
                previousRelationshipComponent.Next = meshChild;
            }

            if (!relationshipComponent.First) {
                relationshipComponent.First = meshChild;
            }

            previous = meshChild;
        }

        for (uint32_t i = 0; i < node->mNumChildren; i++)
        {
            //pNode->AddChild(ProcessNode(node->mChildren[i], scene));
            Entity child = ProcessNodeECS(node->mChildren[i], scene, walkerScene);
            child.GetComponent<RelationshipComponent>().Parent = entity;

            if (!relationshipComponent.First) {
                relationshipComponent.First = child;
                previous = child;
            } else if (previous) {
                RelationshipComponent& previousRelationshipComponent = previous.GetComponent<RelationshipComponent>();
                previousRelationshipComponent.Next = child;
                RelationshipComponent& nextRelationshipComponent = child.GetComponent<RelationshipComponent>();
                nextRelationshipComponent.Previous = previous;
            }
        }

        return entity;
    }

    std::shared_ptr<Mesh> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
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
        return std::make_shared<Mesh>(mesh->mName.C_Str(), vertices, indices, m_Materials[mesh->mMaterialIndex]);
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