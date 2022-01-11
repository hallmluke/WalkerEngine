#version 330 core
out float FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

uniform vec3 samples[64];
uniform mat4 projection;
uniform mat4 view;

// tile noise texture over screen, based on screen dimensions / noise size
const vec2 noiseScale = vec2(1280/4.0, 720/4.0); // screen = 800x600

void main()
{

	vec3 fragPos = texture(gPosition, TexCoords).xyz;
	vec3 normal = texture(gNormal, TexCoords).rgb;
	vec3 randomVec = texture(texNoise, TexCoords * noiseScale * 2).xyz;

	vec4 fragPosViewSpace = (view * vec4(texture(gPosition, TexCoords).xyz, 1.0));
	vec4 normalViewSpace = (view * vec4(texture(gNormal, TexCoords).rgb, 0.0));

	vec3 tangent = normalize(randomVec - normalViewSpace.xyz * dot(randomVec, normalViewSpace.xyz));
	vec3 bitangent = cross(normalViewSpace.xyz, tangent);
	mat3 TBN = mat3(tangent, bitangent, normalViewSpace.xyz);


	int kernelSize = 16;
	float radius = 2;
	float occlusion = 0.0;
	float bias = 0.025;

	//vec4 FragPosClipSpace = projection * fragPosViewSpace;

	//FragColor = normalize(normalViewSpace).z;

	for(int i = 0; i < kernelSize; ++i)
	{
		// get sample position
		vec3 sample = TBN * samples[i]; // tangent/world space // from tangent to view-space
		sample = fragPosViewSpace.xyz + sample * radius; // world space
		vec4 offset = vec4(sample, 1.0);
		offset = projection * offset; // world space to clip space // from view to clip-space
		offset.xyz /= offset.w; // perspective divide
		offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0

		//float sampleDepth = texture(gPosition, offset.xy).z;
		float sampleDepth = (view * vec4(texture(gPosition, offset.xy).xyz, 1.0)).z; // Make sure we're getting in view space
		float rangeCheck = smoothstep(0.0, 1.0, radius /
							abs(fragPosViewSpace.z - sampleDepth));
		occlusion += (sampleDepth >= sample.z + bias ? 1.0 : 0.0) * rangeCheck;

	}

	occlusion = 1.0 - (occlusion / kernelSize);
	FragColor = occlusion;

}