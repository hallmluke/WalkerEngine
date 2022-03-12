#version 450 core
layout(rgba16f, binding = 0) uniform image3D voxelTex;

in vec3 FragPos;
//in mat3 TBN;
//in vec3 NormalFrag;
//in vec2 TexCoordsFrag;
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

uint layers = 128;

ivec3 scaleAndBias(vec3 position) { return ivec3((position + vec3(imageSize(voxelTex).xy, layers) / 2)); }

bool IsInProbe(vec3 position, ivec3 size) { 
    return abs(position.x) < size.x / 2 && abs(position.y) < size.y / 2 && abs(position.z) < size.z / 2; 
}

void main() {
	
	//if(!IsInProbe(FragPos, imageSize(voxelTex))) return;

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
    //vec4 diffuseColor = texture(texture_diffuse1, TexCoordsFrag);
    //vec4 diffuseColor = vec4(1.0, 0.0, 0.0, 1.0);

    //ivec3 voxel = ivec3(64, 64, 64);

    //imageStore(voxelTex, voxel, diffuseColor);
    vec4 diffuseColor = texture(texture_diffuse1, TexCoords);
    ivec3 voxel = scaleAndBias(FragPos);
    ivec3 size = imageSize(voxelTex);

    bool greaterThanZero = voxel.x >= 0 && voxel.y >= 0 && voxel.z >= 0;
    bool xIsGood = voxel.x < size.x;
    bool yIsGood = voxel.y < size.y;
    bool zIsGood = voxel.z < layers;
    //float z = size.z;
    ///diffuseColor.z = z;
    //diffuseColor.y = 1.5;

    if(greaterThanZero && xIsGood && yIsGood && zIsGood) {
    //if(voxel.x >= 0 && voxel.x < size.x && voxel.y >= 0 && voxel.y < size.y && voxel.z > 0 && voxel.z < size.z) {
        imageStore(voxelTex, ivec3(voxel), diffuseColor);
    }
}