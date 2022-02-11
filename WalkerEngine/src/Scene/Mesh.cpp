#include "walkerpch.h"
#include "Mesh.h"
#include "Renderer/Renderer.h"

namespace Walker {

    Mesh::Mesh(const std::string name, std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::shared_ptr<Material> material) {
        m_Name = name;
        m_Vertices = vertices;
        m_Indices = indices;
        m_Material = material;

        SetupMesh();
    }

    void Mesh::Draw(std::shared_ptr<Shader> shader, glm::mat4 transform)
    {
        // TODO: Refactor texture binding
        m_Material->BindTextures();
        shader->SetInt("texture_diffuse1", 0);
        shader->SetInt("texture_metallicRoughness1", 1);
        shader->SetInt("texture_normal1", 2);
        Renderer::Submit(shader, m_VertexArray, transform);
    }

	void Mesh::SetupMesh()
	{
        m_VertexArray = VertexArray::Create();
        m_VertexBuffer = VertexBuffer::Create((float*) m_Vertices.data(), m_Vertices.size() * sizeof(Vertex));

        BufferLayout layout = {
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float3, "a_Normal" },
            { ShaderDataType::Float2, "a_TexCoords" },
            { ShaderDataType::Float3, "a_Tangent" },
            { ShaderDataType::Float3, "a_Bitangent" }
        };
        m_VertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(m_VertexBuffer);

        m_IndexBuffer = IndexBuffer::Create(m_Indices.data(), m_Indices.size());
        m_VertexArray->SetIndexBuffer(m_IndexBuffer);
        
	}
}
