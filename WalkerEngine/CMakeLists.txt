FILE(GLOB_RECURSE SRCFILES src/*.cpp src/*.h)
add_library(WalkerEngine ${SRCFILES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

include_directories(src/)
include_directories(vendor/)
include_directories(vendor/entt/include)
add_compile_definitions(W_PLATFORM_WINDOWS)

set( GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set( GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")
add_subdirectory(vendor/GLFW3)
add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/Assimp)
add_subdirectory(vendor/GLM)
add_subdirectory(vendor/glad)
add_subdirectory(vendor/ImGui)

target_include_directories(${PROJECT_NAME} PUBLIC vendor/GLFW3 vendor/spdlog vendor/Assimp vendor/GLM vendor/ImGui vendor/entt/include vendor/glad/include)

target_link_libraries(${PROJECT_NAME} PUBLIC glfw spdlog assimp ImGui Glad)