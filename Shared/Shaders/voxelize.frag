#version 450 core
layout(rgba8, binding = 0) uniform image3D voxelTex;

in vec3 FragPos;
in mat3 TBN;
in vec2 TexCoords;

uniform bool diffuse_tex;
uniform sampler2D texture_diffuse1;
uniform bool specular_tex;
uniform sampler2D texture_specular1;
uniform bool normal_tex;
uniform sampler2D texture_normal1;


struct DirLight {
    vec3 direction;
    vec3 color;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    mat4 lightSpaceMatrix;
    sampler2D shadowMap;
    float shadowBias;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    samplerCube depthMap;
    float far_plane;
    float bias;
};

const int MAX_LIGHTS = 8;
uniform int numberOfLights;
uniform PointLight lights[MAX_LIGHTS];
uniform DirLight dirLight;

vec3 scaleAndBias(vec3 position) { return (position + imageSize(voxelTex) / 2); }

bool IsInProbe(vec3 position, ivec3 size) { 
    return abs(position.x) < size.x / 2 && abs(position.y) < size.y / 2 && abs(position.z) < size.z / 2; 
}

void main() {
	
	if(!IsInProbe(FragPos, imageSize(voxelTex))) return;

    //vec3 lightDir = normalize(-dirLight.direction);
    //
    //vec3 normal;
    //if(normal_tex) {
    //    normal = texture(texture_normal1, TexCoords).rgb;
    //} else {
    //    normal = vec3(0.0, 0.0, 1.0);
    //}
    //normal = normal * 2.0 - 1.0;
    //normal = normalize(TBN * normal);
    //
    //float diffuse = max(dot(normal, lightDir), 0.0);
    vec4 diffuseColor = texture(texture_diffuse1, TexCoords);

    vec3 voxel = scaleAndBias(FragPos);

    imageStore(voxelTex, ivec3(voxel), diffuseColor);
}