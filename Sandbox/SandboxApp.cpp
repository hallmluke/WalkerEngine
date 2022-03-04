#include <Walker.h>
#include "Core/EntryPoint.h"
#include "SandboxLayer.h"

namespace Walker {
	class Sandbox : public Application
	{
	public:
		Sandbox() : Application("Sandbox")
		{
			PushLayer(new SandboxLayer());
		}

		~Sandbox()
		{

		}
	};

	Application* CreateApplication() {
		return new Sandbox();
	}
}