#include "walkerpch.h"
#include "Skybox.h"

#include "Renderer/RenderCommand.h"
#include "Math/SampleGeometry/Cube.h"


namespace Walker {

	Skybox::Skybox(std::string directory)
	{
		m_TextureCubeMap = TextureCubeMap::Create(directory);
		m_SkyboxShader = Shader::Create("Skybox", "Shaders/skybox.vert", "Shaders/skybox.frag");
		InitBuffers();
	}

	void Skybox::InitBuffers()
	{
		std::vector<float> vertices = Cube::GetUnindexedVertices();

		m_VertexArray = VertexArray::Create();
		m_VertexBuffer = VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(float));

		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoords" }
		};
		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
	}

	void Skybox::Draw(glm::mat4 view, glm::mat4 projection)
	{
		// Set global state
		RenderCommand::SetDepthFunction(RendererAPI::DepthFunction::LEQUAL);
		RenderCommand::DisableBackfaceCulling();

		m_SkyboxShader->Bind();
		view = glm::mat4(glm::mat3(view)); // remove translation from the view matrix
		m_SkyboxShader->SetMat4("view", view);
		m_SkyboxShader->SetMat4("projection", projection);
		m_SkyboxShader->SetInt("skybox", 0);
		m_TextureCubeMap->Bind(0);
		m_VertexArray->Bind();
		RenderCommand::DrawUnindexed(m_VertexArray, 36);

		// Revert global state
		RenderCommand::SetDepthFunction(RendererAPI::DepthFunction::LESS);
		RenderCommand::EnableBackfaceCulling();
	}
}