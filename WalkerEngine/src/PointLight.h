#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <vector>

class Shader;
class Model;

namespace Walker {

	class PointLight {
	public:
		PointLight(glm::vec3 pos);
		//void DrawDebug(Shader& shader);
		//void ControlWindow();



		//float debugDrawSize = 0.2f;
		//bool drawDebugEnabled = true;
		//bool debugShadow = false;

		//unsigned int VBO, VAO;
		//std::vector<float> vertices;

		// Shadow mapping
		/*bool shadowMapEnabled = true;
		unsigned int depthCubeMap;
		unsigned int depthMapFBO;
		float nearPlane = 0.1f;
		float farPlane = 25.0f;
		float aspect = 1.0f;
		float bias = 0.05f;
		glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, nearPlane, farPlane);
		void GenerateShadowMap(Shader& shader, Model& model);*/

	private:
		std::string m_Name;

		glm::vec3 m_Position;
		float m_AmbientIntensity;
		float m_DiffuseIntensity;
		float m_SpecularIntensity;

		float m_ConstantAttenuation;
		float m_LinearAttenuation;
		float m_QuadraticAttenuation;

		/*bool initialized = false;
		void InitBuffers();

		// Shadow mapping
		const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
		bool shadowMapInitialized = false;
		void InitShadowMap();*/
	};
}