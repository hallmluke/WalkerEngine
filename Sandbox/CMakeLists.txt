FILE(GLOB_RECURSE SRCFILES *.cpp *.h)
add_executable(Sandbox ${SRCFILES})

set_property(TARGET Sandbox PROPERTY CXX_STANDARD 20)
add_compile_definitions(W_PLATFORM_WINDOWS)

find_package(Vulkan REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/WalkerEngine/src/)
target_link_libraries(Sandbox WalkerEngine Vulkan::Vulkan VulkanMemoryAllocator)

find_program(GLSLC glslc)

FILE(GLOB_RECURSE SHADERS ${CMAKE_SOURCE_DIR}/Shared/VulkanShaders/*.vert ${CMAKE_SOURCE_DIR}/Shared/VulkanShaders/*.frag ${CMAKE_SOURCE_DIR}/Shared/VulkanShaders/*.geom ${CMAKE_SOURCE_DIR}/Shared/VulkanShaders/*.comp)
foreach(shader_file ${SHADERS})
    message(STATUS ${shader_file})
    get_filename_component(shader-name ${shader_file} NAME)
    add_custom_command(
           OUTPUT Shaders/${shader-name}.spv
           COMMAND ${GLSLC} -o Shaders/${shader-name}.spv ${shader_file}
           DEPENDS ${shader_file}
           IMPLICIT_DEPENDS CXX ${shader_file}
           VERBATIM)
    set_source_files_properties(Shaders/${shader-name}.spv PROPERTIES GENERATED TRUE)
    list(APPEND SPIRV_BINARY_FILES Shaders/${shader-name}.spv)
endforeach()

FILE(GLOB_RECURSE SHADER_HEADERS ${CMAKE_SOURCE_DIR}/Shared/VulkanShaders/*.glsl)
add_custom_target(
    Shaders
    SOURCES ${SHADERS} ${SHADER_HEADERS}
    DEPENDS ${SPIRV_BINARY_FILES}
    )
source_group(TREE ${CMAKE_SOURCE_DIR}/Shared FILES ${SHADERS} ${SHADER_HEADERS})

#set_property(TARGET Sandbox PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:Sandbox>")

# TODO: Remove second commands. Currently done so Visual Studio can find assets
#add_custom_command(TARGET Sandbox POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shared/Shaders/ $<TARGET_FILE_DIR:Sandbox>/Shaders/)
#add_custom_command(TARGET Sandbox POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shared/Shaders/ $<TARGET_FILE_DIR:Sandbox>/../Shaders/)

#Sadly, symlinks require escalate priveleges on Windows which makes this a pain. Sigh...
#add_custom_command(TARGET Sandbox POST_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/Shared/Shaders/ $<TARGET_FILE_DIR:Sandbox>/Shaders/)
#add_custom_command(TARGET Sandbox POST_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/Shared/Shaders/ $<TARGET_FILE_DIR:Sandbox>/../Shaders/)

add_custom_command(TARGET Sandbox POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shared/Assets/Icons/ $<TARGET_FILE_DIR:Sandbox>/Icons/)
#add_custom_command(TARGET Sandbox POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shared/Assets/Icons/ $<TARGET_FILE_DIR:Sandbox>/../Icons/)

add_custom_command(TARGET Sandbox POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shared/Assets/Models/ $<TARGET_FILE_DIR:Sandbox>/Models/)
#add_custom_command(TARGET Sandbox POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shared/Assets/Models/ $<TARGET_FILE_DIR:Sandbox>/../Models/)

add_custom_command(TARGET Sandbox POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shared/Assets/Skybox/ $<TARGET_FILE_DIR:Sandbox>/Skybox/)
#add_custom_command(TARGET Sandbox POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shared/Assets/Skybox/ $<TARGET_FILE_DIR:Sandbox>/../Skybox/)

add_custom_command(TARGET Sandbox POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shared/Assets/Fonts/ $<TARGET_FILE_DIR:Sandbox>/Fonts/)
#add_custom_command(TARGET Sandbox POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shared/Assets/Fonts/ $<TARGET_FILE_DIR:Sandbox>/../Fonts/)

add_custom_command(TARGET Sandbox POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shared/Assets/Scenes/ $<TARGET_FILE_DIR:Sandbox>/Scenes/)
#add_custom_command(TARGET Sandbox POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shared/Assets/Scenes/ $<TARGET_FILE_DIR:Sandbox>/../Scenes/)

add_dependencies(Sandbox Shaders)