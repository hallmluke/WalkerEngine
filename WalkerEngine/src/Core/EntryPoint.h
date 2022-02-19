#pragma once

#ifdef W_PLATFORM_WINDOWS

extern Walker::Application* Walker::CreateApplication();

int main(int argc, char** arg)
{
	Walker::Log::Init();
	W_CORE_WARN("Initialized log");
	auto app = Walker::CreateApplication();
	app->Run();
	delete app;
}
#endif