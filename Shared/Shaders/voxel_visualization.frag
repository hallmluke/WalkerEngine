#version 450 core
out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;

uniform sampler3D volumeTex;

uniform float stepSize;
uniform int steps;
uniform float radius;
uniform float opacityThreshold;
uniform vec3 origin;

bool IsInVolume(vec3 pos) {
    return abs(pos.x) < 17 && abs(pos.y) < 17 && abs(pos.z) < 17;
}

void main()
{
    vec3 viewDir = normalize(FragPos - viewPos);

    vec4 color = vec4(0.0);

    vec3 startPosition = IsInVolume(viewPos) ? viewPos : FragPos;

    for(uint step = 0; step < 200 && color.a < 0.9; step++) {
        
        vec3 samplePoint = startPosition + viewDir * step * 0.1;

        //samplePoint += 64;
        //samplePoint /= 128;
        //samplePoint /= 4;
        samplePoint += 16;
        samplePoint /= 32;

        vec4 val = texture(volumeTex, samplePoint);
        color.xyz += val.xyz * val.a;
        color.a += val.z;
    }

    color.a = 1.0;
    FragColor = color;
    //FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
