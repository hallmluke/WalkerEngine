#version 330 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out vec4 gMetRoughAO;

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
    //FragColor = vec4(texture(texture_diffuse1, TexCoords).xyz, 1);
    gPosition = vec4(FragPos, 1.0);
    gAlbedo = vec4(texture(texture_diffuse1, TexCoords).xyz, 1);
    vec3 normal = texture(texture_normal1, TexCoords).rgb;
    normal = normal * 2.0 - 1.0;
    gNormal = vec4(normalize(TBN * normal), 1.0);

    gMetRoughAO.r = texture(texture_metallicRoughness1, TexCoords).b;
    gMetRoughAO.g = texture(texture_metallicRoughness1, TexCoords).g;
    gMetRoughAO.b = 0.0;
    gMetRoughAO.a = 1.0;
}