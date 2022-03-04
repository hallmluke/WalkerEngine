FILE(GLOB_RECURSE SRCFILES *.cpp *.h)
add_executable(Sandbox ${SRCFILES})

set_property(TARGET Sandbox PROPERTY CXX_STANDARD 20)
add_compile_definitions(W_PLATFORM_WINDOWS)

include_directories(${CMAKE_SOURCE_DIR}/WalkerEngine/src/)
target_link_libraries(Sandbox WalkerEngine)

# TODO: Remove second commands. Currently done so Visual Studio can find assets
add_custom_command(TARGET Sandbox POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shared/Shaders/ $<TARGET_FILE_DIR:Sandbox>/Shaders/)
add_custom_command(TARGET Sandbox POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shared/Shaders/ $<TARGET_FILE_DIR:Sandbox>/../Shaders/)

add_custom_command(TARGET Sandbox POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shared/Assets/Icons/ $<TARGET_FILE_DIR:Sandbox>/Icons/)
add_custom_command(TARGET Sandbox POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shared/Assets/Icons/ $<TARGET_FILE_DIR:Sandbox>/../Icons/)

add_custom_command(TARGET Sandbox POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shared/Assets/Models/ $<TARGET_FILE_DIR:Sandbox>/Models/)
add_custom_command(TARGET Sandbox POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shared/Assets/Models/ $<TARGET_FILE_DIR:Sandbox>/../Models/)

add_custom_command(TARGET Sandbox POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shared/Assets/Skybox/ $<TARGET_FILE_DIR:Sandbox>/Skybox/)
add_custom_command(TARGET Sandbox POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shared/Assets/Skybox/ $<TARGET_FILE_DIR:Sandbox>/../Skybox/)

add_custom_command(TARGET Sandbox POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shared/Scenes/ $<TARGET_FILE_DIR:Sandbox>/Scenes/)
add_custom_command(TARGET Sandbox POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shared/Scenes/ $<TARGET_FILE_DIR:Sandbox>/../Scenes/)