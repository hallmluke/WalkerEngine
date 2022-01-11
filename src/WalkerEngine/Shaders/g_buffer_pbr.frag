#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;
layout (location = 3) out vec3 gMetRoughAO;

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
uniform bool roughness_tex;
uniform sampler2D texture_roughness1;

void main()
{    
    if(diffuse_tex) {
        if(texture(texture_diffuse1, TexCoords).a < 0.05) {
            discard;
        }
    }
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
    gAlbedo = texture(texture_diffuse1, TexCoords).rgb;
    // metallic sampled from specular
    if(specular_tex) {
        gMetRoughAO.r = texture(texture_specular1, TexCoords).r;
    } else {
        gMetRoughAO.r = 0.0;
    }
    // roughness
    gMetRoughAO.g = texture(texture_roughness1, TexCoords).g;
    // set AO to 0 for now
    gMetRoughAO.b = 0.0;
}