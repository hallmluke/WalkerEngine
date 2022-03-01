#pragma once

#include "Core/UUID.h"

namespace Walker {

	struct IDComponent
	{
		UUID ID;
		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
		IDComponent(UUID& uuid) : ID(uuid) {}
	};

}