#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec3 FragPos;
out vec2 TexCoords;

uniform sampler3D volumeTex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0)); 
    TexCoords = aTexCoords;
    //inverseModel = inverse(model);
    gl_Position = projection * view * vec4(FragPos, 1.0);
}