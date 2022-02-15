#include "walkerpch.h"
#include "ModelNode.h"

namespace Walker {

	ModelNode::ModelNode(const std::string name, std::vector<std::shared_ptr<Mesh>> meshPtrs, glm::mat4 initialTransform) : name(name)
	{
		this->meshPtrs = std::move(meshPtrs);
		SetInitialTransform(initialTransform);
	}

	void ModelNode::Draw(std::shared_ptr<Shader> shader)//, glm::mat4 accumulatedTransform)
	{
		for (const auto& meshPtr : meshPtrs) {
			meshPtr->Draw(shader, globalTransform);
		}
		for (const auto& childPtr : childNodePtrs) {
			childPtr->Draw(shader);
		}
	}

	void ModelNode::ShowTree(ModelNode*& selectedNode)
	{
		/*int flags = ImGuiTreeNodeFlags_OpenOnArrow | (childNodePtrs.size() == 0 ? ImGuiTreeNodeFlags_Leaf : 0);
		flags = flags | (selectedNode == this ? ImGuiTreeNodeFlags_Selected : 0);

		bool expanded = ImGui::TreeNodeEx(name.c_str(), flags);

		if (ImGui::IsItemClicked()) {
			selectedNode = this;
		}

		if (expanded) {
			for (const auto& child : childNodePtrs) {
				child->ShowTree(selectedNode);
			}

			ImGui::TreePop();
		}*/
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
	}

	void ModelNode::SetRootTransform(glm::mat4 transform) {
		rootTransform = transform;
	}

	void ModelNode::UpdateGlobalTransform()
	{
		globalTransform = rootTransform * initialTransform * appliedTransform.m_transform;

		for (const auto& childPtr : childNodePtrs) {
			childPtr->UpdateGlobalTransform(globalTransform);
		}
	}

	void ModelNode::UpdateGlobalTransform(glm::mat4 parentGlobalTransform)
	{
		globalTransform = parentGlobalTransform * initialTransform * appliedTransform.m_transform;

		for (const auto& childPtr : childNodePtrs) {
			childPtr->UpdateGlobalTransform(globalTransform);
		}
	}

	void ModelNode::ApplyTransform()
	{
		appliedTransform.setAppliedTransform();

		if (parent) {
			UpdateGlobalTransform(parent->globalTransform);
		}
		else {
			UpdateGlobalTransform();
		}
	}

	void ModelNode::ApplyTransform(Transform parentTransform)
	{
		appliedTransform.setAppliedTransform(parentTransform.m_transform);

		for (unsigned int i = 0; i < childNodePtrs.size(); i++) {
			childNodePtrs[i]->ApplyTransform(appliedTransform);
		}
	}
}