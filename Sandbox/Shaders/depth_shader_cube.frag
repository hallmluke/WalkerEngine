#version 330 core
in vec4 FragPos;
in vec2 TexCoords;

uniform vec3 lightPos;
uniform float far_plane;

uniform bool diffuse_tex;
uniform sampler2D texture_diffuse1;

void main()
{
    if(diffuse_tex) {
        if(texture(texture_diffuse1, TexCoords).a < 0.05) {
            discard;
        }
    }

    float lightDistance = length(FragPos.xyz - lightPos);
    
    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / far_plane;
    
    // write this as modified depth
    gl_FragDepth = lightDistance;
}