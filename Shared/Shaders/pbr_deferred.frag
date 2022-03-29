#version 460 core
layout (location = 0) out vec4 gColor;

out vec4 FragColor;

in vec2 TexCoords;

layout (std140, binding = 0) uniform LightSpaceMatrices
{
    mat4 lightSpaceMatrices[16];
};



uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gMetRoughAO;
uniform sampler3D VoxelTex;
//uniform sampler2D ssaoColor;

uniform vec3 camPos;

struct DirLight {
    vec3 direction;
    vec3 color;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float shadowBias;

    mat4 lightSpaceMatrix;
    sampler2D shadowMap;
    
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
uniform sampler2DArray shadowMap;

uniform float cascadePlaneDistances[16];
uniform int cascadeCount;   // number of frusta - 1
uniform float farPlane;

const float PI = 3.14159265359;
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);

uniform int debugPass;
uniform bool useAmbientOcclusion;
uniform bool useVoxelConetrace;
uniform float aperture;
uniform float maxDistance;
uniform float stepSize;
uniform float indirectMultiplier;
uniform int numCones;
uniform float mipModifier;

uniform vec3 GIPosition;
uniform vec3 GIScale;
uniform int GISubdiv;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 albedo, float roughness, float metallic, vec3 F0, vec4 fragPosWorldSpace);
float ShadowCalculationDir(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir);
float ShadowCalculationDirCascades(vec4 fragPosWorldSpace, vec3 normal, vec3 lightDir);
float ShadowCalculationPoint(PointLight light, vec3 fragPos);


//ivec3 scaleAndBias(vec3 position) { return ivec3((position + vec3(64))); }
ivec3 scaleAndBias(vec3 position) { return ivec3((position - GIPosition + GIScale / 2) * GISubdiv / GIScale); }

const vec3 CONES[] = 
{
	vec3(0.57735, 0.57735, 0.57735),
	vec3(0.57735, -0.57735, -0.57735),
	vec3(-0.57735, 0.57735, -0.57735),
	vec3(-0.57735, -0.57735, 0.57735),
	vec3(-0.903007, -0.182696, -0.388844),
	vec3(-0.903007, 0.182696, 0.388844),
	vec3(0.903007, -0.182696, 0.388844),
	vec3(0.903007, 0.182696, -0.388844),
	vec3(-0.388844, -0.903007, -0.182696),
	vec3(0.388844, -0.903007, 0.182696),
	vec3(0.388844, 0.903007, -0.182696),
	vec3(-0.388844, 0.903007, 0.182696),
	vec3(-0.182696, -0.388844, -0.903007),
	vec3(0.182696, 0.388844, -0.903007),
	vec3(-0.182696, 0.388844, 0.903007),
	vec3(0.182696, -0.388844, 0.903007)
};

vec4 ConeTrace(vec3 position, vec3 normal, vec3 coneDirection, float coneAperture)
{
    vec3 color = vec3(0.0);
    float alpha = 0.0;

    bool belowHorizon = dot(normal, coneDirection) < 0;
    coneDirection = belowHorizon ? -coneDirection : coneDirection;

    vec3 startPos = position + normal + coneDirection;
    float dist = 0.0;

    //const float maxDistance = 100.0f;

    while(dist < maxDistance && alpha < 1) {

        float diameter = max(1.0f, 2 * coneAperture * dist);
        float mip = log2( diameter * GISubdiv / GIScale.x * mipModifier); 

        vec3 worldPosition = startPos + coneDirection * dist;

        ivec3 voxelPosition = scaleAndBias(worldPosition);

        //vec3 voxelTexPosition = vec3(voxelPosition) / 128;
        vec3 voxelTexPosition = vec3(voxelPosition) / GISubdiv;

        if(mip >= 5) {
            break;
        }

        vec4 sampled = textureLod(VoxelTex, voxelTexPosition, mip);

        float a = 1 - alpha;
        color += a * sampled.rgb;
        alpha += a * sampled.a;

        dist += diameter * 2.0 * stepSize;
    }

    return vec4(color, 1.0);

}

vec4 ConeTraceRadiance(vec3 position, vec3 normal)
{
    vec4 radiance = vec4(0.0);
    //const float aperture = tan(PI * 0.5 * 0.33);

    for (uint cone = 0; cone < numCones; ++cone) // quality is between 1 and 16 cones
	{
		vec3 coneDirection = normalize(CONES[cone] + normal);

		radiance += ConeTrace(position, normal, coneDirection, aperture);
	}

	radiance /= numCones;
	radiance.a = clamp(radiance.a, 0.0, 1.0);

	return max(vec4(0.0), radiance);
}

void main()
{
    
	vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Albedo = pow(texture(gAlbedo, TexCoords).rgb, vec3(2.2));
    float Metallic = texture(gMetRoughAO, TexCoords).r;
    float Roughness = texture(gMetRoughAO, TexCoords).g;
    float AO = texture(gMetRoughAO, TexCoords).b;
    //float SSAO = texture(ssaoColor, TexCoords).r;

    //vec4 FragPosDirLightSpace = dirLight.lightSpaceMatrix * vec4(FragPos, 1.0);

    //if(debugPass == 0) {
    //    FragColor = vec4(FragPos, 1.0);
    //    return;
    //} else if (debugPass == 1) {
    //    FragColor = vec4(Normal, 1.0);
    //    return;
    //} else if (debugPass == 2) {
    //    FragColor = vec4(Albedo, 1.0);
    //    return;
    //} else if (debugPass == 3) {
    //    FragColor = vec4(Metallic);
    //    FragColor.a = 1.0;
    //    return;
    //} else if (debugPass == 4) {
    //    FragColor = vec4(Roughness);
    //    FragColor.a = 1.0;
    //    return;
    //} else if (debugPass == 5) {
    //    FragColor = vec4(SSAO);
    //    FragColor.a = 1.0;
    //    return;
    //} else if (debugPass == 6) {

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, Albedo, Metallic);

    vec3 N = normalize(Normal);
    vec3 V = normalize(camPos - FragPos);

    vec3 Lo = vec3(0.0);
    for(int i = 0; i < numberOfLights; ++i) {
        vec3 L = normalize(lights[i].position - FragPos);
        vec3 H = normalize(V + L);

        float distance = length(lights[i].position - FragPos);
        //float attenuation = 1.0 / (distance * distance);
        float attenuation = 1.0 / (lights[i].constant + lights[i].linear * distance + lights[i].quadratic * (distance * distance));
        vec3 radiance = lights[i].diffuse * attenuation;
        float shadow = ShadowCalculationPoint(lights[i], FragPos);
        radiance = ( 1 - shadow ) * radiance;


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
        
        //float shadow = 0;
        vec3 final = (kD * Albedo / PI + specular) * radiance * NdotL;
        //Lo += final;
    }

    Lo += CalcDirLight(dirLight, N, V, Albedo, Roughness, Metallic, F0, vec4(FragPos, 1.0));

    
    if(useVoxelConetrace) {
        vec3 radiance = ConeTraceRadiance(FragPos, N).xyz * indirectMultiplier;
        Lo += radiance * Albedo;
    }

    //float averageAO = (AO + SSAO) / 2;
    vec3 ambient = vec3(0.00) * Albedo;
    //ambient = vec3(0.0);
    //if(useAmbientOcclusion) {
        //ambient = ambient * SSAO;
    //}
    //vec3 ambient = vec3(0.03) * Albedo * averageAO;
    vec3 color = ambient + Lo;

    //color = color / (color + vec3(1.0)); // Tone map (HDR)
    //color = pow(color, vec3(1.0/2.2)); // Gamma correct
    //color = clamp(color, 0.0, 1.0);
    FragColor = vec4(color, 1.0);
    //if(useVoxelConetrace) {
    //    FragColor = ConeTraceRadiance(FragPos, N);
    //}
    gColor = FragColor;

    //}
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 albedo, float roughness, float metallic, vec3 F0, vec4 fragPosWorldSpace) {
    vec3 L = normalize(-light.direction);
    vec3 H = normalize(viewDir + L);

    vec3 radiance = light.color * light.diffuse;

    // cook-torrance brdf
    float NDF = DistributionGGX(normal, H, roughness);
    float G = GeometrySmith(normal, viewDir, L, roughness);
    vec3 F = fresnelSchlick(max(dot(H, viewDir), 0.0), F0);
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;
    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, L), 0.0);
    vec3 specular = numerator / max(denominator, 0.001);
    float NdotL = max(dot(normal, L), 0.0);
    //float shadow = ShadowCalculationDir(fragPosLightSpace, normal, L);
    float shadow = ShadowCalculationDirCascades(fragPosWorldSpace, normal, L);
    //float shadow = 0;
    vec3 final = (1 - shadow) * (kD * albedo / PI + specular) * radiance * NdotL;

    return final;
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
    float bias = max(0.005 * (1.0 - dot(normal, lightDir)), dirLight.shadowBias);
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