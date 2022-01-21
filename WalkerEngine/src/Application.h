#pragma once
#include "Core.h"

namespace Walker {

	class WALKER_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	Application* CreateApplication();
}