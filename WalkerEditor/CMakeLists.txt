FILE(GLOB_RECURSE SRCFILES *.cpp *.h)
add_executable(WalkerEditor ${SRCFILES})

set_property(TARGET WalkerEditor PROPERTY CXX_STANDARD 20)
add_compile_definitions(W_PLATFORM_WINDOWS)

include_directories(${CMAKE_SOURCE_DIR}/WalkerEngine/src/)
target_link_libraries(WalkerEditor WalkerEngine)

# TODO: Remove second commands. Currently done so Visual Studio can find assets
add_custom_command(TARGET WalkerEditor POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shared/Shaders/ $<TARGET_FILE_DIR:WalkerEditor>/Shaders/)
add_custom_command(TARGET WalkerEditor POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shared/Shaders/ $<TARGET_FILE_DIR:WalkerEditor>/../Shaders/)

add_custom_command(TARGET WalkerEditor POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shared/Assets/Icons/ $<TARGET_FILE_DIR:WalkerEditor>/Icons/)
add_custom_command(TARGET WalkerEditor POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shared/Assets/Icons/ $<TARGET_FILE_DIR:WalkerEditor>/../Icons/)

add_custom_command(TARGET WalkerEditor POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shared/Assets/Models/ $<TARGET_FILE_DIR:WalkerEditor>/Models/)
add_custom_command(TARGET WalkerEditor POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shared/Assets/Models/ $<TARGET_FILE_DIR:WalkerEditor>/../Models/)

add_custom_command(TARGET WalkerEditor POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shared/Assets/Skybox/ $<TARGET_FILE_DIR:WalkerEditor>/Skybox/)
add_custom_command(TARGET WalkerEditor POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shared/Assets/Skybox/ $<TARGET_FILE_DIR:WalkerEditor>/../Skybox/)

add_custom_command(TARGET WalkerEditor POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shared/Assets/Fonts/ $<TARGET_FILE_DIR:WalkerEditor>/Fonts/)
add_custom_command(TARGET WalkerEditor POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shared/Assets/Fonts/ $<TARGET_FILE_DIR:WalkerEditor>/../Fonts/)

add_custom_command(TARGET WalkerEditor POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shared/Scenes/ $<TARGET_FILE_DIR:WalkerEditor>/Scenes/)
add_custom_command(TARGET WalkerEditor POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shared/Scenes/ $<TARGET_FILE_DIR:WalkerEditor>/../Scenes/)