#pragma once

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "ImGui/backends/imgui_impl_vulkan.h"

class ImGuiManager {
public:
	ImGuiManager(GLFWwindow* window);
	~ImGuiManager();
	void BeginFrame();
	void EndFrame();
};