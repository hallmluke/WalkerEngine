#version 460 core

layout (location = 0) out vec4 outColor;

uniform sampler2D u_Position;
uniform sampler2D u_InFocusColor;
uniform sampler2D u_OutOfFocusColor;

uniform float u_MinDistance;
uniform float u_MaxDistance;

uniform mat4 view;

out vec4 fragColor;

void main() {
	
  vec2 texSize  = textureSize(u_InFocusColor, 0).xy;
  vec2 texCoord = gl_FragCoord.xy / texSize;

  vec4 positionViewSpace = (view * vec4(texture(u_Position, texCoord).xyz, 1.0));

  vec4 inFocusColor = texture(u_InFocusColor, texCoord);
  vec4 outFocusColor = texture(u_OutOfFocusColor, texCoord);

  float blur = smoothstep(u_MinDistance, u_MaxDistance, positionViewSpace.z);

  fragColor = mix(inFocusColor, outFocusColor, blur);
  outColor = fragColor;

}