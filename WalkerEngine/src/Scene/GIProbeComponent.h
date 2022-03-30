#pragma once
#include "GIProbe.h"

namespace Walker {

	struct GIProbeComponent
	{
		std::shared_ptr<GIProbe> Probe;
		GIProbeComponent() {
			Probe = std::make_shared<GIProbe>();
		}
		GIProbeComponent(std::shared_ptr<GIProbe> probe) : Probe(probe) {}
	};
}