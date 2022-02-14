#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"

class Shader;

namespace Walker {
	class Skybox {
	public:
		Skybox(std::string directory);
		void Draw(glm::mat4 view, glm::mat4 projection);

	private:
		std::shared_ptr<TextureCubeMap> m_TextureCubeMap;
		std::shared_ptr<VertexArray> m_VertexArray;
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		std::shared_ptr<Shader> m_SkyboxShader;

		void InitBuffers();
		
		/*Skybox(std::string directory);
		void Draw(glm::mat4 view, glm::mat4 projection);
		void InitBuffers();
		void InitTextures(std::string directory);

		unsigned int skyboxVAO, skyboxVBO;
		unsigned int textureID;
		Shader skyboxShader = Shader("Shaders/skybox.vert", "Shaders/skybox.frag");
		std::vector<std::string> faces{ "right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg" };*/
	};
}