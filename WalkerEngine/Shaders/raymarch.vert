#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec3 FragPos;
out vec2 TexCoords;
//out mat4 inverseModel;

uniform sampler3D volumeTex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 inverseModel;
uniform vec3 volumeBegin;
uniform vec3 volumeEnd;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0)); 
    TexCoords = aTexCoords;
    //inverseModel = inverse(model);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}