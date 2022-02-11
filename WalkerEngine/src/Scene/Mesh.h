#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Shader.h"
#include "Renderer/Material.h"
#include "Math/Transform.h"

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
        Mesh(const std::string name, std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::shared_ptr<Material> material);

        // render the mesh
        void Draw(std::shared_ptr<Shader> shader, glm::mat4 transform);

    private:
        std::string m_Name;
        std::vector<Vertex> m_Vertices;
        std::vector<uint32_t> m_Indices;
        std::shared_ptr<VertexArray> m_VertexArray;
        std::shared_ptr<VertexBuffer> m_VertexBuffer;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;
        std::shared_ptr<Material> m_Material;

        void SetupMesh();
    };
}