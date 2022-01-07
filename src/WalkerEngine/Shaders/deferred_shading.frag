#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform int debugPass;

//struct Light {
//   vec3 Position;
//    vec3 Color;
//    
//    float Linear;
//    float Quadratic;
//};

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
uniform vec3 viewPos;
uniform DirLight dirLight;

//vec4 FragPosDirLightSpace;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 diffuseInput, float specularInput, vec4 fragPositionLightSpace);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseInput, float specularInput);
//vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

float ShadowCalculationDir(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir);
float ShadowCalculationPoint(PointLight light, vec3 fragPos);

void main()
{   
    
    // retrieve data from gbuffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;

    vec4 FragPosDirLightSpace = dirLight.lightSpaceMatrix * vec4(FragPos, 1.0);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    if(debugPass == 0) {
        FragColor = vec4(FragPos, 1.0);
    } else if (debugPass == 1) {
        FragColor = vec4(Normal, 1.0);
    } else if (debugPass == 2) {
        FragColor = vec4(Diffuse, 1.0);
    } else if (debugPass == 3) {
        FragColor = vec4(Specular);
        FragColor.a = 1.0;
    } else if (debugPass == 4) {
        // Actually render
        vec3 result = CalcDirLight(dirLight, Normal, viewDir, Diffuse, Specular, FragPosDirLightSpace);
        for(int i = 0; i < numberOfLights; i++) {
            result += CalcPointLight(lights[i], Normal, FragPos, viewDir, Diffuse, Specular); 
            //FragColor = vec4(normalize(vec3(lights[i].position)), 1.0);
        }

        //vec3 projCoords = FragPosDirLightSpace.xyz / FragPosDirLightSpace.w;
        //projCoords = projCoords * 0.5 + 0.5;

        //float closestDepth = texture(dirLight.shadowMap, projCoords.xy).r;

        //FragColor = vec4(vec3(closestDepth), 1.0);

        //FragColor = vec4(CalcPointLight(lights[0], Normal, FragPos, viewDir, Diffuse, Specular), 1.0);
        FragColor = vec4(result, 1.0);
        //FragColor = vec4(normalize(vec3(lights[0].position)), 1.0);
    }
    // then calculate lighting as usual
    //vec3 lighting  = Diffuse * 0.1; // hard-coded ambient component
    //vec3 viewDir  = normalize(viewPos - FragPos);
    //for(int i = 0; i < NR_LIGHTS; ++i)
    //{
        // diffuse
    //    vec3 lightDir = normalize(lights[i].Position - FragPos);
    //    vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * lights[i].Color;
        // specular
     //   vec3 halfwayDir = normalize(lightDir + viewDir);  
     //   float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
     //   vec3 specular = lights[i].Color * spec * Specular;
        // attenuation
     //   float distance = length(lights[i].Position - FragPos);
     //   float attenuation = 1.0 / (1.0 + lights[i].Linear * distance + lights[i].Quadratic * distance * distance);
     //   diffuse *= attenuation;
     //   specular *= attenuation;
     //   lighting += diffuse + specular;        
    //}
    
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 diffuseInput, float specularInput, vec4 fragPositionLightSpace)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // combine results
    vec3 ambient = light.ambient * diffuseInput;
    vec3 diffuse = light.diffuse * diff * diffuseInput;
    vec3 specular = light.specular * spec * vec3(specularInput);

    float shadow = ShadowCalculationDir(fragPositionLightSpace, normal, lightDir);

    return (ambient + (1 - shadow) * (diffuse + specular));
    //return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseInput, float specularInput)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * diffuseInput;
    vec3 diffuse = light.diffuse * diff * diffuseInput;
    vec3 specColor = vec3(specularInput);

    vec3 specular = light.specular * spec * specColor;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    float shadow = ShadowCalculationPoint(light, fragPos);

    return (ambient + (1 - shadow) * (diffuse + specular));
    //return ambient + diffuse + specular;
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

    float viewDistance = length(viewPos - fragPos);
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