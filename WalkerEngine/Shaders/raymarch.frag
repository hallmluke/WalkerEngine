#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoords;
//in mat4 inverseModel;

uniform mat4 inverseModel;
uniform vec3 viewPos;

uniform sampler3D volumeTex;

uniform float stepSize;
uniform int steps;
uniform float radius;
uniform float opacityThreshold;
uniform vec3 origin;


float sphIntersect( vec3 ro, vec3 rd, vec4 sph )
{
    vec3 oc = ro - sph.xyz;
    float b = dot( oc, rd );
    float c = dot( oc, oc ) - sph.w*sph.w;
    float h = b*b - c;
    if( h<0.0 ) return -1.0;
    h = sqrt( h );
    return -b - h;
}

void main()
{
    vec3 viewDir = normalize(FragPos - viewPos);

    vec3 color = vec3(0.5);
    float opacity = 0;

    vec4 sphere = vec4(origin, radius);

    //float hit = sphIntersect(viewPos, viewDir, sphere);

    //if(hit < 0) {
    //    discard;
    //}
   

    for(int i=0; i < steps; i++) {
        vec3 samplePoint = FragPos + viewDir * stepSize * i;
        vec3 samplePointModelSpace = (inverseModel * vec4(samplePoint, 1.0)).xyz;
        vec3 normalizedSamplePoint = samplePointModelSpace + vec3(0.5);
        //vec3 normalizedSamplePoint = samplePoint + 0.5;

        //float dist = distance(normalizedSamplePoint, origin);
        //float dist = length(normalizedSamplePoint);
        //if(dist < radius) {
        //    opacity += 0.1;
        //}

        if(normalizedSamplePoint.x > 1 ||
           normalizedSamplePoint.y > 1 ||
           normalizedSamplePoint.z > 1 ||
           normalizedSamplePoint.x < 0 ||
           normalizedSamplePoint.y < 0 ||
           normalizedSamplePoint.z < 0) {
           break;
        }

        opacity += texture(volumeTex, normalizedSamplePoint).r;
    }

    if(opacity < opacityThreshold) {
        discard;
    }

    FragColor = vec4(color, 1.0);
}

