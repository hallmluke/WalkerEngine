#include <Walker.h>
#include "Core/EntryPoint.h"
#include "EditorLayer.h"

namespace Walker {

	class WalkerEditor : public Application
	{
	public:
		WalkerEditor() : Application("Walker Editor")
		{
			PushLayer(new EditorLayer());
		}

		~WalkerEditor()
		{

		}
	};

	Application* CreateApplication() {
		return new WalkerEditor();
	}
}