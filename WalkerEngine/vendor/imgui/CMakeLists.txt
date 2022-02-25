FILE(GLOB SRCFILES *.cpp **/*.cpp *.h **/*.h)
add_library(ImGui "imconfig.h"
"imgui.h"
"imgui.cpp"
"imgui_draw.cpp"
"imgui_internal.h"
"imgui_widgets.cpp"
"imstb_rectpack.h"
"imstb_textedit.h"
"imstb_truetype.h"
"imgui_demo.cpp"
"imgui_tables.cpp"
"backends/imgui_impl_glfw.h"
"backends/imgui_impl_glfw.cpp"
"backends/imgui_impl_opengl3.cpp"
"backends/imgui_impl_opengl3.h")
set_property(TARGET ImGui PROPERTY CXX_STANDARD 20)

include_directories(/)

target_include_directories(ImGui PUBLIC ../vendor/GLFW3 ../vendor/glad/include)

target_link_libraries(ImGui PUBLIC glfw Glad)