FILE(GLOB_RECURSE SRCFILES *.cpp *.h)
add_executable(WalkerEditor ${SRCFILES})

set_property(TARGET WalkerEditor PROPERTY CXX_STANDARD 20)
add_compile_definitions(W_PLATFORM_WINDOWS)

include_directories(${CMAKE_SOURCE_DIR}/WalkerEngine/src/)
target_link_libraries(WalkerEditor WalkerEngine)