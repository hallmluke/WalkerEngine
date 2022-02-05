#pragma once

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "imgui/imgui.h";

#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Shader.h"
#include "Renderer/Material.h"
#include "Transform.h"


/*#define MAX_BONE_INFLUENCE 4

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    //int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    //float m_Weights[MAX_BONE_INFLUENCE];
};*/

/*
struct Texture {
    unsigned int id;
    string type;
    string path;
    bool transparency;
};*/

namespace Walker {

    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
        glm::vec3 Tangent;
        glm::vec3 Bitangent;
    };

    class Mesh {
    public:
        
        //bool transparency;
        //unsigned int VAO;

        Mesh(const std::string name, std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::shared_ptr<Material> material);

        // render the mesh
        void Draw(std::shared_ptr<Shader> shader, glm::mat4 transform);
        /* {
            // bind appropriate textures
            unsigned int diffuseNr = 1;
            unsigned int specularNr = 1;
            unsigned int normalNr = 1;
            unsigned int heightNr = 1;
            unsigned int roughnessNr = 1;
            unsigned int metallicRoughnessNr = 1;

            //glm::mat4 transposed = glm::transpose(transform.m_transform);

            shader.setMat4("model", transform);
            shader.setBool("diffuse_tex", false);
            shader.setBool("specular_tex", false);
            shader.setBool("normal_tex", false);

            for (unsigned int i = 0; i < textures.size(); i++)
            {
                glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
                // retrieve texture number (the N in diffuse_textureN)
                string number;
                string name = textures[i].type;
                if (name == "texture_diffuse") {
                    number = std::to_string(diffuseNr++);
                    shader.setBool("diffuse_tex", true);
                }
                else if (name == "texture_specular") {
                    number = std::to_string(specularNr++); // transfer unsigned int to string
                    shader.setBool("specular_tex", true);
                }
                else if (name == "texture_normal") {
                    number = std::to_string(normalNr++); // transfer unsigned int to string
                    shader.setBool("normal_tex", true);
                }
                else if (name == "texture_height") {
                    number = std::to_string(heightNr++); // transfer unsigned int to string
                }
                else if (name == "texture_roughness") {
                    number = std::to_string(roughnessNr++);
                    shader.setBool("roughness_tex", true);
                }
                else if (name == "texture_metallicRoughness") {
                    number = std::to_string(metallicRoughnessNr++);
                    shader.setBool("metallicRoughness_tex", true);
                }

                // now set the sampler to the correct texture unit
                glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
                // and finally bind the texture
                glBindTexture(GL_TEXTURE_2D, textures[i].id);
            }

            // draw mesh
            glBindVertexArray(VAO);

            if (transparency) {
                glDisable(GL_CULL_FACE);
            }

            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            // always good practice to set everything back to defaults once configured.
            glActiveTexture(GL_TEXTURE0);
            if (transparency) {
                glEnable(GL_CULL_FACE);
            }
        }*/

        /*void ControlWindow() {
            if (ImGui::TreeNode(name.c_str())) {
                ImGui::TreePop();
            }
        }*/

    private:
        std::string m_Name;
        // mesh Data
        std::vector<Vertex> m_Vertices;
        std::vector<uint32_t> m_Indices;
        //vector<Texture>      textures;
        std::shared_ptr<VertexArray> m_VertexArray;
        std::shared_ptr<VertexBuffer> m_VertexBuffer;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;
        std::shared_ptr<Material> m_Material;
        // render data 
        //unsigned int VBO, EBO;

        // initializes all the buffer objects/arrays
        void SetupMesh();
    };
}