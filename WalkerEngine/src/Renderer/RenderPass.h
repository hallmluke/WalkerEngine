#pragma once

namespace Walker {

	struct RenderPassInput {
		RenderPassInput() = default;
		RenderPassInput(std::string inputName, uint32_t inputSlot)
			: Name(inputName), Slot(inputSlot) {}
		std::string Name;
		uint32_t Slot;
	};

	class RenderPass {
	public:
		virtual ~RenderPass() = default;
		virtual void BindInputs() const = 0;
		virtual void Draw() const = 0;
		virtual std::vector<RenderPassInput> GetInputs() const = 0;

	};
}