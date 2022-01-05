#include "ModelNode.h"
#include "Mesh.h"
#include "Shader.h"

ModelNode::ModelNode(const std::string name, std::vector<std::unique_ptr<Mesh>> meshPtrs, glm::mat4 initialTransform) : name(name)
{
	this->meshPtrs = std::move(meshPtrs);
	SetInitialTransform(initialTransform);
}

void ModelNode::Draw(Shader& shader, glm::mat4 accumulatedTransform)
{
	std::cout << name << std::endl;
	//glm::mat4 builtTransform = appliedTransform.m_transform * initialTransform * accumulatedTransform;
	glm::mat4 builtTransform = accumulatedTransform * initialTransform * appliedTransform.m_transform;

	for (const auto& meshPtr : meshPtrs) {
		meshPtr->Draw(shader, builtTransform);
	}
	for (const auto& childPtr : childNodePtrs) {
		childPtr->Draw(shader, builtTransform);
	}
}

void ModelNode::ShowTree()
{
	if (ImGui::TreeNodeEx(name.c_str())) {
		for (const auto& child : childNodePtrs) {
			child->ShowTree();
		}

		ImGui::TreePop();
	}
}

void ModelNode::AddChild(std::unique_ptr<ModelNode> modelNode)
{
	if (modelNode) {
		modelNode->parent = this;
		childNodePtrs.push_back(std::move(modelNode));
	}
}

void ModelNode::SetInitialTransform(glm::mat4 transform)
{
	initialTransform = transform;

	/*if (parent) {
		glm::mat4 applied = parent->appliedTransform.m_transform * transform.m_transform;
		appliedTransform.m_transform = applied;
	}
	else {
		appliedTransform = transform;
	}*/

	//ApplyTransform();
}

void ModelNode::ApplyTransform()
{
	appliedTransform.setAppliedTransform();

	/*for (unsigned int i = 0; i < childNodePtrs.size(); i++) {
		childNodePtrs[i]->ApplyTransform(appliedTransform);
	}*/
}

void ModelNode::ApplyTransform(Transform parentTransform)
{
	std::cout << this->name << std::endl;
	appliedTransform.setAppliedTransform(parentTransform.m_transform);

	for (unsigned int i = 0; i < childNodePtrs.size(); i++) {
		childNodePtrs[i]->ApplyTransform(appliedTransform);
	}
}
