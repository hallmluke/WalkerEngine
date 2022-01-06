#pragma once

#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include "Transform.h"

class Mesh;
class Shader;

class ModelNode {
public:
	ModelNode(const std::string name, std::vector<std::unique_ptr<Mesh>> meshPtrs, glm::mat4 initialTransform);
	void Draw(Shader& shader);// , glm::mat4 transform);
	void ShowTree(ModelNode*& selectedNode);
	void AddChild(std::unique_ptr<ModelNode> modelNode);
	void SetInitialTransform(glm::mat4 transform);
	void SetRootTransform(glm::mat4 transform);
	void UpdateGlobalTransform();
	void UpdateGlobalTransform(glm::mat4 parentGlobalTransform);
	void ApplyTransform();
	void ApplyTransform(Transform parentTransform);
	ModelNode* parent;
	Transform appliedTransform;

private:
	int id;
	std::string name;
	std::vector<std::unique_ptr<ModelNode>> childNodePtrs;
	std::vector<std::unique_ptr<Mesh>> meshPtrs;
	glm::mat4 initialTransform;
	glm::mat4 globalTransform;
	glm::mat4 rootTransform;
	
};