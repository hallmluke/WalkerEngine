#version 450 core

layout (location = 0) out vec4 gColor;
layout (location = 1) out int gEntities;

in vec3 FragPos;
in mat3 TBN;
in vec2 TexCoords;
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

uniform int entityId;

void main()
{   
    FragColor = vec4(0.04, 0.28, 0.26, 1.0);
    gColor = FragColor;
    gEntities = entityId;
}