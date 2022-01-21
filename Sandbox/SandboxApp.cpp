#include <Walker.h>

class Sandbox : public Walker::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

Walker::Application* Walker::CreateApplication() {
	return new Sandbox();
}