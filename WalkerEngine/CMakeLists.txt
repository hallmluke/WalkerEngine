FILE(GLOB_RECURSE SRCFILES src/*.cpp src/*.h)
FILE(GLOB_RECURSE SHADERS ../Shared/Shaders/*.vert ../Shared/Shaders/*.frag ../Shared/Shaders/*.geom ../Shared/Shaders/*.comp)
add_library(WalkerEngine ${SRCFILES} ${SHADERS})
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${SRCFILES})
source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${SHADERS})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

include_directories(src/)
include_directories(vendor/)
include_directories(vendor/entt/include)
add_compile_definitions(W_PLATFORM_WINDOWS)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DW_DEBUG")

set( GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set( GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")
add_subdirectory(vendor/GLFW3)
set_property(TARGET glfw PROPERTY FOLDER External)
set_property(TARGET update_mappings PROPERTY FOLDER External)

add_subdirectory(vendor/spdlog)
set_property(TARGET spdlog_headers_for_ide PROPERTY FOLDER External)
set_property(TARGET spdlog-utests PROPERTY FOLDER External)


set_property(TARGET Continuous PROPERTY FOLDER External)
set_property(TARGET Experimental PROPERTY FOLDER External)
set_property(TARGET Nightly PROPERTY FOLDER External)
set_property(TARGET NightlyMemoryCheck PROPERTY FOLDER External)

add_subdirectory(vendor/Assimp)

set_property(TARGET assimp PROPERTY FOLDER External)
set_property(TARGET UpdateAssimpLibsDebugSymbolsAndDLLs PROPERTY FOLDER External)
set_property(TARGET assimp_cmd PROPERTY FOLDER External)
set_property(TARGET unit PROPERTY FOLDER External)
set_property(TARGET uninstall PROPERTY FOLDER External)
set_property(TARGET zlibstatic PROPERTY FOLDER External)


add_subdirectory(vendor/GLM)

add_subdirectory(vendor/glad)
set_property(TARGET Glad PROPERTY FOLDER External)

add_subdirectory(vendor/ImGui)
set_property(TARGET ImGui PROPERTY FOLDER External)

add_subdirectory(vendor/stb)
set_property(TARGET stb PROPERTY FOLDER External)

set(YAML_BUILD_SHARED_LIBS OFF CACHE BOOL "Use static lib for yaml-cpp")
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "Do not build tests")
add_subdirectory(vendor/yaml-cpp)
set_property(TARGET yaml-cpp PROPERTY FOLDER External)
set_property(TARGET format PROPERTY FOLDER External)
set_property(TARGET parse PROPERTY FOLDER External)
set_property(TARGET read PROPERTY FOLDER External)
#set_property(TARGET run-tests PROPERTY FOLDER External)
set_property(TARGET sandbox PROPERTY FOLDER External)

target_include_directories(${PROJECT_NAME} PUBLIC vendor/GLFW3 vendor/spdlog vendor/Assimp vendor/GLM vendor/ImGui vendor/entt/include vendor/glad/include vendor/stb/include vendor/yaml-cpp/include)

target_link_libraries(${PROJECT_NAME} PUBLIC glfw spdlog assimp ImGui Glad stb yaml-cpp)

#TODO: Precompiled headers 
#target_precompile_headers(WalkerEngine PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/src/walkerpch.h>")