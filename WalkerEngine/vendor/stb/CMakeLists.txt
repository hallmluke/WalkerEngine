add_library(stb "include/stb_image.h" "src/stb_image.cpp")
set_property(TARGET stb PROPERTY CXX_STANDARD 20)

include_directories(include/)