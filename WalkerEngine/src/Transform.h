#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>


class Transform
{
public:
	glm::vec3 m_pos = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_eulerRotation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_scale = { 1.0f, 1.0f, 1.0f };

	glm::mat4 m_transform = glm::mat4(1.0f);

	glm::mat4 getLocalModelMatrix();


    void setAppliedTransform();
	void setAppliedTransform(const glm::mat4& parentGlobalModelMatrix);

	void aiMatrix4x4ToGlm(aiMatrix4x4 from)
	{
		m_transform[0][0] = from.a1; m_transform[0][1] = from.b1;  m_transform[0][2] = from.c1; m_transform[0][3] = from.d1;
		m_transform[1][0] = from.a2; m_transform[1][1] = from.b2;  m_transform[1][2] = from.c2; m_transform[1][3] = from.d2;
		m_transform[2][0] = from.a3; m_transform[2][1] = from.b3;  m_transform[2][2] = from.c3; m_transform[2][3] = from.d3;
		m_transform[3][0] = from.a4; m_transform[3][1] = from.b4;  m_transform[3][2] = from.c4; m_transform[3][3] = from.d4;
	}
};