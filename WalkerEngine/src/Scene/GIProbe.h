#pragma once
#include <memory>
#include <stdint.h>
#include <Renderer/Texture.h>

namespace Walker {

	class GIProbe {
	public:
		std::shared_ptr<Texture3D> VoxelTex;
		uint32_t Subdiv = 128;
		float Energy = 1.0f;

		GIProbe() = default;
	private:

	};
}