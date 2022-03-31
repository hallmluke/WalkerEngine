#version 450 core
layout(rgba16f, binding = 1) uniform image3D voxelTex;

out vec4 FragColor;

in vec3 FragPos;
//in mat3 TBN;
in vec3 NormalFrag;
in vec2 TexCoordsFrag;
//in vec2 TexCoords;
in vec4 FragPosLightSpace;

struct VoxelType
{
    uint color;
    uint normal;
};

layout (std140, binding = 1) buffer VoxelBuffer {
    VoxelType voxels[];
};

uint flatten3D(uvec3 coord, uvec3 dim)
{
    return (coord.z * dim.x * dim.y) + (coord.y * dim.x) + coord.x;
}

// Encode HDR color to a 32 bit uint
// Alpha is 1 bit + 7 bit HDR remapping
uint EncodeColor(in vec4 color)
{
	// normalize color to LDR
	float hdr = length(color.rgb);
	color.rgb /= hdr;

	// encode LDR color and HDR range
	uvec3 iColor = uvec3(color.rgb * 255.0f);
	uint iHDR = uint(clamp(hdr / 10.0f, 0.0, 1.0) * 127);
	uint colorMask = (iHDR << 24u) | (iColor.r << 16u) | (iColor.g << 8u) | iColor.b;

	// encode alpha into highest bit
	uint iAlpha = (color.a > 0 ? 1u : 0u);
	colorMask |= iAlpha << 31u;

	return colorMask;
}


// Encode specified normal (normalized) into an unsigned integer. Each axis of
// the normal is encoded into 9 bits (1 for the sign/ 8 for the value).
uint EncodeNormal(in vec3 normal)
{
	ivec3 iNormal = ivec3(normal*255.0f);
	uvec3 iNormalSigns;
	iNormalSigns.x = (iNormal.x >> 5) & 0x04000000;
	iNormalSigns.y = (iNormal.y >> 14) & 0x00020000;
	iNormalSigns.z = (iNormal.z >> 23) & 0x00000100;
	iNormal = abs(iNormal);
	uint normalMask = iNormalSigns.x | (iNormal.x << 18) | iNormalSigns.y | (iNormal.y << 9) | iNormalSigns.z | iNormal.z;
	return normalMask;
}



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

uniform mat4 view;
uniform float farPlane;

uniform vec3 GIPosition;
uniform vec3 GIScale;
uniform int GISubdiv;

ivec3 scaleAndBias(vec3 position) { return ivec3((position - GIPosition + GIScale / 2) * GISubdiv / GIScale); }

bool IsInProbe(vec3 position, ivec3 size) { 
    return abs(position.x) < size.x / 2 && abs(position.y) < size.y / 2 && abs(position.z) < size.z / 2; 
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec4 fragPosWorldSpace);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos);
//float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir);

void main() {

    vec4 diffuseColor = texture(texture_diffuse1, TexCoordsFrag);

    vec3 diffuseLuminance = vec3(0.0);
    for(int i = 0; i < numberOfLights; ++i) {
        //diffuseLuminance += CalcPointLight(lights[i], NormalFrag, FragPos);
    }
    diffuseLuminance += CalcDirLight(dirLight, NormalFrag, FragPosLightSpace);
    ivec3 voxel = scaleAndBias(FragPos);
    //ivec3 size = imageSize(voxelTex);
    ivec3 size = ivec3(GISubdiv);

    bool greaterThanZero = voxel.x >= 0 && voxel.y >= 0 && voxel.z >= 0;
    bool xIsGood = voxel.x < size.x;
    bool yIsGood = voxel.y < size.y;
    bool zIsGood = voxel.z < size.z;
    //bool zIsGood = voxel.z < layers;
    //float z = size.z;
    ///diffuseColor.z = z;
    //diffuseColor.y = 1.5;

    if(greaterThanZero && xIsGood && yIsGood && zIsGood) {
    //if(voxel.x >= 0 && voxel.x < size.x && voxel.y >= 0 && voxel.y < size.y && voxel.z > 0 && voxel.z < size.z) {
        //imageStore(voxelTex, ivec3(voxel), vec4(diffuseLuminance, 1.0));
        //uint id = flatten3D(uvec3(voxel), uvec3(128));
        uint id = flatten3D(uvec3(voxel), uvec3(GISubdiv));
        vec4 lightedDiffuseColor = vec4(diffuseLuminance, 1.0);
        uint encodedColor = EncodeColor(lightedDiffuseColor);
        uint encodedNormal = EncodeNormal(NormalFrag);
        atomicMax(voxels[id].color, encodedColor);
        atomicMax(voxels[id].normal, encodedNormal);
    }

    FragColor = vec4(diffuseColor);
}

vec3 sampleOffsetDirections[20] = vec3[]
(
vec3( 1, 1, 1), vec3( 1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
vec3( 1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
vec3( 1, 1, 0), vec3( 1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
vec3( 1, 0, 1), vec3(-1, 0, 1), vec3( 1, 0, -1), vec3(-1, 0, -1),
vec3( 0, 1, 1), vec3( 0, -1, 1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float ShadowCalculationPoint(PointLight light, vec3 fragPos)
{
    //vec3 fragToLight = FragPos - light.position;
    vec3 fragToLight = FragPos - light.position;
    float closestDepth = texture(light.depthMap, fragToLight).r;
    closestDepth *= light.far_plane;
    float currentDepth = length(fragToLight);
    //float bias = 0.002;
    //float shadow = currentDepth - light.bias > closestDepth ? 1.0 : 0.0;
    float shadow = 0.0;
    int samples = 20;

    float viewDistance = 4;
    float diskRadius = (1.0 + (viewDistance / light.far_plane)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(light.depthMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= light.far_plane; // undo mapping [0;1]
        if(currentDepth - light.bias > closestDepth) {
            shadow += 1.0;
        }
     }

    shadow /= float(samples);

    return shadow;
    //return closestDepth / light.far_plane;
    //return currentDepth / light.far_plane;
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
    // TODO
    // Figure out why the "diff" term seems to cause odd issues
    //vec3 diffuse = light.diffuse * vec3(texture(texture_diffuse1, TexCoordsFrag));
    vec3 specColor = vec3(0.2);
    if(specular_tex) {
        specColor = vec3(texture(texture_specular1, TexCoordsFrag));
    }

    //vec3 specular = light.specular * spec * specColor;
    ambient *= attenuation;
    diffuse *= attenuation;
    //specular *= attenuation;

    float shadow = ShadowCalculationPoint(light, fragPos);

    //return (ambient + (1 - shadow) * (diffuse + specular));

    return (1 - shadow) * diffuse;
    //return diffuse;
}


float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir) {
    
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(dirLight.shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z; 
    float bias = max(0.005 * (1.0 - dot(normal, lightDir)), dirLight.shadowBias);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(dirLight.shadowMap, 0);

    for(int x = -2; x <= 2; ++x)
    {
        for(int y = -2; y <= 2; ++y)
        {
            float pcfDepth = texture(dirLight.shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - dirLight.shadowBias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 25;

    if(projCoords.z > 1.0) {
        shadow = 0.0;
    }

    return shadow;

}

vec3 CalcDirLight(DirLight light, vec3 normal, vec4 fragPosWorldSpace) {
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    //vec3 reflectDir = reflect(-lightDir, normal);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoordsFrag));

    //vec3 diffuse = vec3(texture(texture_diffuse1, TexCoordsFrag));
    //vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords).r);

    float shadow = ShadowCalculation(FragPosLightSpace, normal, lightDir);

    //vec3 final = diffuse;
    vec3 final = (1 - shadow) * diffuse;

    return final;
}