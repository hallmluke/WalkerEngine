cmake_minimum_required(VERSION 3.22.2)
project(WalkerEngine)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

function(add_shader TARGET SHADER DIRECTORY)
    find_program(GLSLC glslc)
    get_filename_component(shader-name ${SHADER} NAME)

    #set(current-shader-path ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/${SHADER})
    set(current-shader-path ${SHADER})
    #set(current-output-path ${CMAKE_BINARY_DIR}/shaders/${shader-name}.spv)
    #set(current-output-path ${DIRECTORY}/${shader-name}.spv)

    # Add a custom command to compile GLSL to SPIR-V.
    get_filename_component(current-output-dir ${DIRECTORY}/${shader-name}.spv DIRECTORY)
    #file(MAKE_DIRECTORY ${current-output-dir})

    add_custom_command(
           OUTPUT ${DIRECTORY}/${shader-name}.spv
           COMMAND ${GLSLC} -o ${DIRECTORY}/${shader-name}.spv ${current-shader-path}
           DEPENDS ${current-shader-path}
           IMPLICIT_DEPENDS CXX ${current-shader-path}
           VERBATIM)

    # Make sure our build depends on this output.
    #set_source_files_properties(${current-output-path} PROPERTIES GENERATED TRUE)
    #target_sources(${TARGET} PRIVATE ${current-output-path})
endfunction(add_shader)

add_subdirectory(WalkerEngine)
add_subdirectory(WalkerEditor)
add_subdirectory(Sandbox) 