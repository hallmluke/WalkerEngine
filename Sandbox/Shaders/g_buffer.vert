#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec3 FragPos;
//out vec3 Normal;
out mat3 TBN;
out vec2 TexCoords;
//out vec4 FragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
//uniform mat4 lightSpaceMatrix;

void main()
{
    
    // View Space
    //vec4 viewPos = view * model * vec4(aPos, 1.0);
    //FragPos = viewPos.xyz;
    //vec3 T = normalize(vec3(view * model * vec4(aTangent, 0.0)));
    //vec3 B = normalize(vec3(view * model * vec4(aBitangent, 0.0)));
    // vec3 N = normalize(vec3(view * model * vec4(aNormal, 0.0)));
    // gl_Position = projection * viewPos;


    // Global Position
    FragPos = vec3(model * vec4(aPos, 1.0));
    vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
    
    vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
    
    vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
    
    TBN = mat3(T, B, N);

    //Normal = mat3(transpose(inverse(model))) * aNormal;  
    TexCoords = aTexCoords;
    //FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
    
}