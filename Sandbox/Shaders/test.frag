#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;
layout (location = 3) out vec3 gMetRoughAO;

in vec3 FragPos;
in mat3 TBN;
in vec2 TexCoords;
//in vec3 Normal;
out vec4 FragColor;

uniform bool diffuse_tex;
uniform sampler2D texture_diffuse1;
uniform bool specular_tex;
uniform sampler2D texture_specular1;
uniform bool normal_tex;
uniform sampler2D texture_normal1;
uniform bool roughness_tex;
uniform sampler2D texture_roughness1;
uniform bool metallicRoughness_tex;
uniform sampler2D texture_metallicRoughness1;

void main()
{    
    FragColor = vec4(texture(texture_diffuse1, TexCoords).xyz, 1);
}