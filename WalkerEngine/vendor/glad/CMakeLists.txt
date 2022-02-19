add_library(Glad "include/glad/glad.h" "include/KHR/khrplatform.h" "src/glad.c")
set_property(TARGET Glad PROPERTY CXX_STANDARD 20)

include_directories(include/)