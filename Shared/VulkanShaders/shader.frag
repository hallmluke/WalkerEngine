#version 450
#include "testinclude.glsl"

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D texSampler;

void main() {
    doStuff();
    outColor = texture(texSampler, fragTexCoord);
}  