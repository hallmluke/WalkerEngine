#include "walkerpch.h"
#include "Window.h"

#ifdef W_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Walker {

	Window* Window::Create(const WindowProps& props)
	{
		#ifdef W_PLATFORM_WINDOWS
		return new WindowsWindow(props);
#else

		return nullptr;
#endif
	}

}
