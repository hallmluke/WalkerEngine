#version 460 core

layout (location = 0) out vec4 gColor;

uniform sampler2D u_ColorTexture;

out vec4 fragColor;

void main() {
	
  //  gColor = vec4(1.0, 0.0, 0.0, 1.0);
  vec2 texSize  = textureSize(u_ColorTexture, 0).xy;
  vec2 texCoord = gl_FragCoord.xy / texSize;

  // TODO: uniform, configurable
  int size = 3;

  fragColor.rgb = vec3(0.0);
  float count = 0.0;
  
  for (int i = -size; i <= size; i++) {
    for (int j = -size; j <= size; j++) {
  
        vec2 offset = vec2(i, j) / texSize;
        fragColor.rgb += texture(u_ColorTexture, texCoord + offset).rgb;
        count += 1.0;
  
     }
   }
  
   fragColor.a = 1;
   fragColor.rgb /= count;
   gColor = fragColor;

}