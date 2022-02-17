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
/*
Skybox::Skybox(std::string directory)
{
	InitBuffers();
	InitTextures(directory);
}

void Skybox::Draw(glm::mat4 view, glm::mat4 projection)
{
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	
	// Disable face culling as we render inside face
	glDisable(GL_CULL_FACE);
	skyboxShader.use();
	view = glm::mat4(glm::mat3(view)); // remove translation from the view matrix
	skyboxShader.setMat4("view", view);
	skyboxShader.setMat4("projection", projection);
	skyboxShader.setInt("skybox", 0);
	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
	glEnable(GL_CULL_FACE); // re-enable face culling
}

void Skybox::InitBuffers()
{
	vector<float> vertices = Cube::getUnindexedVertices();

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
}

void Skybox::InitTextures(std::string directory)
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load((directory + "/" + faces[i]).c_str(), &width, &height,
			&nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
				width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap failed to load at path: " << directory + "/" + faces[i]
				<< std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,
		GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,
		GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,
		GL_CLAMP_TO_EDGE);
}
*/