#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;


in vec3 FragPos;
in mat3 TBN;
in vec2 TexCoords;
//in vec3 Normal;

uniform bool diffuse_tex;
uniform sampler2D texture_diffuse1;
uniform bool specular_tex;
uniform sampler2D texture_specular1;
uniform bool normal_tex;
uniform sampler2D texture_normal1;

void main()
{    
    // store the fragment position vector in the first gbuffer texture
    gPosition = FragPos;
    // also store the per-fragment normals into the gbuffer
    vec3 normal;
    if(normal_tex) {
        normal = texture(texture_normal1, TexCoords).rgb;
    } else {
        normal = vec3(0.0, 0.0, 1.0);
    }
    normal = normal * 2.0 - 1.0;
    gNormal = normalize(TBN * normal);
    // and the diffuse per-fragment color
    gAlbedoSpec.rgb = texture(texture_diffuse1, TexCoords).rgb;
    // store specular intensity in gAlbedoSpec's alpha component
    gAlbedoSpec.a = texture(texture_specular1, TexCoords).r;
}