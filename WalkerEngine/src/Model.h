#pragma once
#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h""
#include "Shader.h"
#include "Transform.h"
#include "ModelNode.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

class Transform;

using namespace std;

unsigned int TextureFromFile(const char* path, const string& directory, bool& transparency, bool gamma = false);

class Model
{
public:
    std::string name;
    //Transform transform;
    // model data 
    vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    //vector<Mesh>    meshes;
    string directory;
    bool gammaCorrection;

    // constructor, expects a filepath to a 3D model.
    Model(const std::string name, string const& path, glm::mat4 initialTransform, bool gamma = false);

    // draws the model, and thus all its meshes
    void Draw(Shader& shader);

    void ControlWindow();
    void SetGlobalTransformOnNodes();

private:
    std::unique_ptr<ModelNode> rootNode;
    glm::mat4 baseTransform;
    ModelNode* selectedNode;
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string const& path);

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    std::unique_ptr<ModelNode> processNode(aiNode* node, const aiScene* scene);

    std::unique_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};
#endif