#version 330 core
//out vec4 FragColor;
//
//struct Material {
//    sampler2D diffuse;
//    sampler2D specular;
//    float shininess;
//}; 
//
//struct DirLight {
//    vec3 direction;
//
//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;
//};
//
//struct PointLight {
//    vec3 position;
//    
//    float constant;
//    float linear;
//    float quadratic;
//
//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;
//
//    samplerCube depthMap;
//    float far_plane;
//    float bias;
//};
//
//struct SpotLight {
//    vec3 position;
//    vec3 direction;
//    float cutOff;
//    float outerCutOff;
//  
//    float constant;
//    float linear;
//    float quadratic;
//  
//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;
//};
//
//#define NR_POINT_LIGHTS 4
//
//in vec3 FragPos;
//in mat3 TBN;
//in vec2 TexCoords;
//in vec4 FragPosLightSpace;
//
//uniform vec3 viewPos;
//uniform DirLight dirLight;
////uniform PointLight pointLights[NR_POINT_LIGHTS];
//uniform PointLight pointLight;
////uniform SpotLight spotLight;
////uniform Material material;
//uniform bool debugShadow;
//
//vec3 sampleOffsetDirections[20] = vec3[]
//(
//vec3( 1, 1, 1), vec3( 1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
//vec3( 1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
//vec3( 1, 1, 0), vec3( 1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
//vec3( 1, 0, 1), vec3(-1, 0, 1), vec3( 1, 0, -1), vec3(-1, 0, -1),
//vec3( 0, 1, 1), vec3( 0, -1, 1), vec3( 0, -1, -1), vec3( 0, 1, -1)
//);
//
//
//uniform float shadowBias;
//
//uniform bool diffuse_tex;
//uniform sampler2D texture_diffuse1;
//uniform bool specular_tex;
//uniform sampler2D texture_specular1;
//uniform bool normal_tex;
//uniform sampler2D texture_normal1;
//
//uniform sampler2D shadowMap;
//
//// function prototypes
//vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
//vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
//vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
//
//float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir) {
//    
//    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
//    projCoords = projCoords * 0.5 + 0.5;
//
//    float closestDepth = texture(shadowMap, projCoords.xy).r;
//    float currentDepth = projCoords.z; 
//    float bias = max(0.005 * (1.0 - dot(normal, lightDir)), shadowBias);
//
//    float shadow = 0.0;
//    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
//
//    for(int x = -2; x <= 2; ++x)
//    {
//        for(int y = -2; y <= 2; ++y)
//        {
//            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
//            shadow += currentDepth - shadowBias > pcfDepth ? 1.0 : 0.0;
//        }
//    }
//    shadow /= 25;
//
//    if(projCoords.z > 1.0) {
//        shadow = 0.0;
//    }
//
//    return shadow;
//
//}
//
//float ShadowCalculationPoint(PointLight light, vec3 fragPos)
//{
//    //vec3 fragToLight = FragPos - light.position;
//    vec3 fragToLight = FragPos - light.position;
//    float closestDepth = texture(light.depthMap, fragToLight).r;
//    closestDepth *= light.far_plane;
//    float currentDepth = length(fragToLight);
//    //float bias = 0.002;
//    //float shadow = currentDepth - light.bias > closestDepth ? 1.0 : 0.0;
//    float shadow = 0.0;
//    int samples = 20;
//
//    float viewDistance = length(viewPos - fragPos);
//    float diskRadius = (1.0 + (viewDistance / light.far_plane)) / 25.0;
//    for(int i = 0; i < samples; ++i)
//    {
//        float closestDepth = texture(light.depthMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
//        closestDepth *= light.far_plane; // undo mapping [0;1]
//        if(currentDepth - light.bias > closestDepth) {
//            shadow += 1.0;
//        }
//     }
//
//    shadow /= float(samples);
//
//    return shadow;
//    //return closestDepth / light.far_plane;
//    //return currentDepth / light.far_plane;
//}
//
void main()
{   
//    if(diffuse_tex) {
//        if(texture(texture_diffuse1, TexCoords).a < 0.05) {
//            discard;
//        }
//    }
//    // properties
//    //vec3 norm = normalize(Normal);
//    vec3 norm;
//    if(normal_tex) {
//        norm = texture(texture_normal1, TexCoords).rgb;
//    } else {
//        norm = vec3(1.0);
//    }
//    norm = norm * 2.0 - 1.0;
//    norm = normalize(TBN * norm);
//    
//    vec3 viewDir = normalize(viewPos - FragPos);
//    
//    // == =====================================================
//    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
//    // For each phase, a calculate function is defined that calculates the corresponding color
//    // per lamp. In the main() function we take all the calculated colors and sum them up for
//    // this fragment's final color.
//    // == =====================================================
//    // phase 1: directional lighting
//    vec3 result = CalcDirLight(dirLight, norm, viewDir);
//    // phase 2: point lights
//    //for(int i = 0; i < NR_POINT_LIGHTS; i++)
//    result += CalcPointLight(pointLight, norm, FragPos, viewDir);    
//    // phase 3: spot light
//    //result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
//    
//    //FragColor = vec4(result, 1.0);
//    //FragColor = vec4(CalcPointLight(pointLight, norm, FragPos, viewDir), 1.0);
//    if(debugShadow) {
//        FragColor = vec4(vec3(length(pointLight.position - FragPos) / pointLight.far_plane), 1.0);
//    } else {
//        FragColor = vec4(result, 1.0);
//    }
}
//
//// calculates the color when using a directional light.
//vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
//{
//    vec3 lightDir = normalize(-light.direction);
//    // diffuse shading
//    float diff = max(dot(normal, lightDir), 0.0);
//    // specular shading
//    vec3 reflectDir = reflect(-lightDir, normal);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
//    // combine results
//    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
//    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
//    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords).r);
//
//    float shadow = ShadowCalculation(FragPosLightSpace, normal, lightDir);
//
//    return (ambient + (1 - shadow) * (diffuse + specular));
//}
//
//// calculates the color when using a point light.
//vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
//{
//    vec3 lightDir = normalize(light.position - fragPos);
//    // diffuse shading
//    float diff = max(dot(normal, lightDir), 0.0);
//    // specular shading
//    vec3 reflectDir = reflect(-lightDir, normal);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
//    // attenuation
//    float distance = length(light.position - fragPos);
//    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
//    // combine results
//    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
//    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
//    vec3 specColor = vec3(0.2);
//    if(specular_tex) {
//        specColor = vec3(texture(texture_specular1, TexCoords));
//    }
//
//    vec3 specular = light.specular * spec * specColor;
//    ambient *= attenuation;
//    diffuse *= attenuation;
//    specular *= attenuation;
//
//    float shadow = ShadowCalculationPoint(pointLight, fragPos);
//
//    return (ambient + (1 - shadow) * (diffuse + specular));
//}
//
//// calculates the color when using a spot light.
//vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
//{
//    vec3 lightDir = normalize(light.position - fragPos);
//    // diffuse shading
//    float diff = max(dot(normal, lightDir), 0.0);
//    // specular shading
//    vec3 reflectDir = reflect(-lightDir, normal);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
//    // attenuation
//    float distance = length(light.position - fragPos);
//    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
//    // spotlight intensity
//    float theta = dot(lightDir, normalize(-light.direction)); 
//    float epsilon = light.cutOff - light.outerCutOff;
//    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
//    // combine results
//    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
//    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
//
//    vec3 specColor = vec3(0.2);
//    if(specular_tex) {
//        specColor = vec3(texture(texture_specular1, TexCoords));
//    }
//
//    vec3 specular = light.specular * spec * specColor;
//    ambient *= attenuation * intensity;
//    diffuse *= attenuation * intensity;
//    specular *= attenuation * intensity;
//    return (ambient + diffuse + specular);
//}