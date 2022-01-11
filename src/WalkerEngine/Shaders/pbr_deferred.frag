#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gMetRoughAO;
uniform sampler2D ssaoColor;

uniform vec3 camPos;

struct DirLight {
    vec3 direction;

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


const int MAX_LIGHTS = 1;
uniform int numberOfLights;
uniform PointLight lights[MAX_LIGHTS];
uniform DirLight dirLight;

const float PI = 3.14159265359;
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);

uniform int debugPass;
uniform bool useAmbientOcclusion;

float ShadowCalculationDir(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir);
float ShadowCalculationPoint(PointLight light, vec3 fragPos);

void main()
{
    
	vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Albedo = texture(gAlbedo, TexCoords).rgb;
    //vec3 Albedo = pow(texture(gAlbedo, TexCoords).rgb, 2.2);
    float Metallic = texture(gMetRoughAO, TexCoords).r;
    float Roughness = texture(gMetRoughAO, TexCoords).g;
    float AO = texture(gMetRoughAO, TexCoords).b;
    float SSAO = texture(ssaoColor, TexCoords).r;

    if(debugPass == 0) {
        FragColor = vec4(FragPos, 1.0);
        return;
    } else if (debugPass == 1) {
        FragColor = vec4(Normal, 1.0);
        return;
    } else if (debugPass == 2) {
        FragColor = vec4(Albedo, 1.0);
        return;
    } else if (debugPass == 3) {
        FragColor = vec4(Metallic);
        FragColor.a = 1.0;
        return;
    } else if (debugPass == 4) {
        FragColor = vec4(Roughness);
        FragColor.a = 1.0;
        return;
    } else if (debugPass == 5) {
        FragColor = vec4(SSAO);
        FragColor.a = 1.0;
        return;
    } else if (debugPass == 6) {

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, Albedo, Metallic);

    vec3 N = normalize(Normal);
    vec3 V = normalize(camPos - FragPos);

    vec3 Lo = vec3(0.0);
    for(int i = 0; i < numberOfLights; ++i) {
        vec3 L = normalize(lights[i].position - FragPos);
        vec3 H = normalize(V + L);

        float distance = length(lights[i].position - FragPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lights[i].diffuse * attenuation;

        // cook-torrance brdf
        float NDF = DistributionGGX(N, H, Roughness);
        float G = GeometrySmith(N, V, L, Roughness);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - Metallic;
        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
        vec3 specular = numerator / max(denominator, 0.001);
        // add to outgoing radiance Lo
        float NdotL = max(dot(N, L), 0.0);
        float shadow = ShadowCalculationPoint(lights[i], FragPos);
        vec3 final = (1 - shadow) * (kD * Albedo / PI + specular) * radiance * NdotL;
        Lo += final;
    }

    float averageAO = (AO + SSAO) / 2;
    vec3 ambient = vec3(0.03) * Albedo * averageAO;
    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0)); // Tone map (HDR)
    color = pow(color, vec3(1.0/2.2)); // Gamma correct
    FragColor = vec4(color, 1.0);
    }
}


vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;
    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}


float ShadowCalculationDir(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir) {
    
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
    vec3 fragToLight = fragPos - light.position;
    float closestDepth = texture(light.depthMap, fragToLight).r;
    closestDepth *= light.far_plane;
    float currentDepth = length(fragToLight);
    //float bias = 0.002;
    //float shadow = currentDepth - light.bias > closestDepth ? 1.0 : 0.0;
    float shadow = 0.0;
    int samples = 20;

    float viewDistance = length(camPos - fragPos);
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