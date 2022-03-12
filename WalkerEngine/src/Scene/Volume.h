#pragma once
#include <vector>
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"
#include "Math/SampleGeometry/Quad.h"
#include "Math/SampleGeometry/Cube.h"

namespace Walker {

	class Volume {
	public:
		Volume(std::shared_ptr<Texture3D> tex);
		void Draw(std::shared_ptr<Shader> shader, glm::mat4 view, glm::mat4 projection, glm::vec3 viewPos);

	private:
		std::shared_ptr<Texture3D> m_VolumeTex;
		std::shared_ptr<VertexArray> m_VertexArray;
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		/*public:
			Volume();
			void InitBuffers();
			void InitTexture();
			void Draw(glm::mat4 view, glm::mat4 projection, glm::vec3 viewPos);
			void ControlWindow();

		private:
			unsigned int VAO, VBO;
			int size = 128;
			unsigned int textureId;
			void PopulateTestData();
			std::vector<uint8_t> data;
			Shader raymarchShader = Shader("Shaders/raymarch.vert", "Shaders/raymarch.frag");
			int steps = 3;
			float stepSize = 0.2;
			float radius = 0.4;
			float opacityThreshold = 0.1;
			glm::vec3 origin;
			Quad quad;*/
	};
}