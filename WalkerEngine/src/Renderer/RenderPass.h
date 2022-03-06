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
		virtual void BindInputs() const;
		virtual void BindOutput(uint32_t outputSlot, uint32_t inputSlot) const = 0;
		virtual void BindOutputImage(uint32_t outputSlot, uint32_t inputSlot) const = 0;

		virtual void DrawScene(Scene& scene) const = 0;

		virtual std::shared_ptr<Framebuffer> GetFramebuffer() const = 0;

		virtual std::vector<RenderPassInput> GetInputs() const;
		virtual RenderPassInput GetInput(std::string name) const;
		virtual RenderPassOutput GetOutput(std::string name) const;

		virtual void LinkToInput(std::string inputName, RenderPassOutput output);

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		// Temp
		virtual uint32_t GetFinalOutputRendererId() const = 0;

	protected:
		std::vector<RenderPassInput> m_Inputs;
		std::vector<RenderPassOutput> m_Outputs;
		std::unordered_map<std::string, RenderPassOutput> m_Links;
	};
}