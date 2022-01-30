#pragma once
#include <sstream>
#include <functional>

#include "Events/Event.h"

namespace Walker {

	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;
		std::string Icon;

		WindowProps(const std::string& title = "Walker Engine",
			uint32_t width = 1600,
			uint32_t height = 900,
			const std::string& icon = "")
			: Title(title), Width(width), Height(height), Icon(icon)
		{
		}
	};

	// Interface representing a desktop system based Window
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		virtual void SetIcon(std::string icon) const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};

}