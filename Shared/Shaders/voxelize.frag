#version 450 core
layout(rgba16f, binding = 1) uniform image3D voxelTex;

out vec4 FragColor;

in vec3 FragPos;
//in mat3 TBN;
in vec3 NormalFrag;
in vec2 TexCoordsFrag;
//in vec2 TexCoords;

layout (std140, binding = 0) uniform LightSpaceMatrices
{
    mat4 lightSpaceMatrices[16];
};

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

   // samplerCube depthMap;
    float far_plane;
    float bias;
};

const int MAX_LIGHTS = 8;
uniform int numberOfLights;
uniform PointLight lights[MAX_LIGHTS];
uniform DirLight dirLight;

uniform mat4 view;
uniform sampler2DArray shadowMap;

uniform float cascadePlaneDistances[16];
uniform int cascadeCount;   // number of frusta - 1
uniform float farPlane;

uint layers = 128;

ivec3 scaleAndBias(vec3 position) { return ivec3((position + vec3(imageSize(voxelTex).xy, layers) / 2)); }

bool IsInProbe(vec3 position, ivec3 size) { 
    return abs(position.x) < size.x / 2 && abs(position.y) < size.y / 2 && abs(position.z) < size.z / 2; 
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec4 fragPosWorldSpace);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos);
float ShadowCalculationDirCascades(vec4 fragPosWorldSpace, vec3 normal, vec3 lightDir);

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

    vec4 diffuseColor = texture(texture_diffuse1, TexCoordsFrag);
    //vec3 diffuseLuminance = CalcDirLight(dirLight, NormalFrag, vec4(FragPos, 1.0));

    vec3 diffuseLuminance = vec3(0.0);
    for(int i = 0; i < numberOfLights; ++i) {
        diffuseLuminance += CalcPointLight(lights[i], NormalFrag, FragPos);
    }
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
        imageStore(voxelTex, ivec3(voxel), vec4(diffuseLuminance, 1.0));
    }

    FragColor = vec4(diffuseColor);
}


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos) {
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoordsFrag));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoordsFrag));
    vec3 specColor = vec3(0.2);
    if(specular_tex) {
        specColor = vec3(texture(texture_specular1, TexCoordsFrag));
    }

    //vec3 specular = light.specular * spec * specColor;
    ambient *= attenuation;
    diffuse *= attenuation;
    //specular *= attenuation;

    //float shadow = ShadowCalculationPoint(light, fragPos);

    //return (ambient + (1 - shadow) * (diffuse + specular));

    return diffuse;
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec4 fragPosWorldSpace) {
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    //vec3 reflectDir = reflect(-lightDir, normal);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoordsFrag));

    //vec3 diffuse = vec3(texture(texture_diffuse1, TexCoordsFrag));
    //vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords).r);

    float shadow = ShadowCalculationDirCascades(fragPosWorldSpace, normal, lightDir);

    vec3 final = diffuse;
    //vec3 final = (1 - shadow) * diffuse;

    return final;
}

float ShadowCalculationDirCascades(vec4 fragPosWorldSpace, vec3 normal, vec3 lightDir) {
    // select cascade layer
    vec4 fragPosViewSpace = view * fragPosWorldSpace;
    float depthValue = abs(fragPosViewSpace.z);

    int layer = -1;
    for (int i = 0; i < cascadeCount; ++i)
    {
        if (depthValue < cascadePlaneDistances[i])
        {
            layer = i;
            break;
        }
    }
    if (layer == -1)
    {
        layer = cascadeCount;
    }

    vec4 fragPosLightSpace = lightSpaceMatrices[layer] * fragPosWorldSpace;
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    if (currentDepth  > 1.0)
    {
        return 0.0;
    }
    // calculate bias (based on depth map resolution and slope)
    float bias = max(0.005 * (1.0 - dot(normal, lightDir)), 0.005);
    if (layer == cascadeCount)
    {
        bias *= 1 / (farPlane * 0.5f);
    }
    else
    {
        bias *= 1 / (cascadePlaneDistances[layer] * 0.5f);
    }

    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / vec2(textureSize(shadowMap, 0));
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, vec3(projCoords.xy + vec2(x, y) * texelSize, layer)).r; 
            shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
    {
        shadow = 0.0;
    }
        
    return shadow;
}