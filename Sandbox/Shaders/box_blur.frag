#version 460 core

layout (location = 0) out vec4 outColor;

uniform sampler2D u_ColorTexture;

out vec4 fragColor;

void main() {
	
  vec2 texSize  = textureSize(u_ColorTexture, 0).xy;
  vec2 texCoord = gl_FragCoord.xy / texSize;

  fragColor = texture(u_ColorTexture, texCoord);

  // TODO: uniform, configurable
  int size = 3;

  fragColor.rgb = vec3(0.0);
  float count = 0.0;

  for (int i = -size; i <= size; ++i) {
    for (int j = -size; j <= size; j++) {
        fragColor.rgb += texture(u_ColorTexture, gl_FragCoord.xy + vec2(i, j) / texSize).rgb;
        count += 1.0;

     }
   }

   fragColor.rgb /= count;
   outColor = fragColor;

}