#pragma once

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

class ImGuiManager {
public:
	ImGuiManager(GLFWwindow* window);
	~ImGuiManager();
	void BeginFrame();
	void EndFrame();
};