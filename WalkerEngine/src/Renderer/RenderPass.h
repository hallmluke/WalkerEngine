#pragma once
#include "Scene/Scene.h"

namespace Walker {

	class RenderPass;

	struct RenderPassOutput {
		RenderPassOutput() = default;
		RenderPassOutput(std::string outputName, uint32_t outputSlot, RenderPass* renderPass)
			: Name(outputName), Slot(outputSlot), RenderPass(renderPass) {}
		std::string Name;
		uint32_t Slot;
		RenderPass* RenderPass;
	};

	struct RenderPassInput {
		RenderPassInput() = default;
		RenderPassInput(std::string inputName, uint32_t inputSlot, RenderPass* renderPass)
			: Name(inputName), Slot(inputSlot), RenderPass(renderPass) {}
		std::string Name;
		uint32_t Slot;
		RenderPass* RenderPass;

		bool operator==(RenderPassInput& rpi) { return Name == rpi.Name; }
	};

	class RenderPass {
	public:
		RenderPass() = default;
		virtual ~RenderPass() = default;
		virtual void BindInputs() const = 0;
		virtual void BindOutput(uint32_t outputSlot, uint32_t inputSlot) const = 0;
		virtual void Draw() const = 0;
		virtual void DrawScene(Scene& scene) const = 0;

		virtual std::vector<RenderPassInput> GetInputs() const = 0;
		virtual RenderPassInput GetInput(std::string name) const = 0;
		virtual RenderPassOutput GetOutput(std::string name) const = 0;

		virtual void LinkToInput(std::string inputName, RenderPassOutput output) = 0;

		// Temp
		virtual uint32_t GetFinalOutputRendererId() const = 0;
	};
}