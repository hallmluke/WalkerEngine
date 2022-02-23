#version 460 core

layout (location = 0) out vec4 gColor;

uniform sampler2D u_Position;
uniform sampler2D u_InFocusColor;
uniform sampler2D u_OutOfFocusColor;

uniform float u_MinDistance;
uniform float u_MaxDistance;

uniform mat4 view;
uniform mat4 projection;

uniform vec3 mouse;

out vec4 fragColor;

in vec2 TexCoords;

void main() {
	
  vec2 texSize  = textureSize(u_InFocusColor, 0).xy;
  vec2 texCoord = gl_FragCoord.xy / texSize;

  vec4 positionViewSpace = (view * vec4(texture(u_Position, TexCoords).xyz, 1.0));

  vec4 inFocusColor = texture(u_InFocusColor, texCoord);
  vec4 outFocusColor = texture(u_OutOfFocusColor, texCoord);

  vec2 center = vec2(0.5, 0.5);
  float focusPoint = -(view * vec4(texture(u_Position, center).xyz, 1.0)).z;

  float blur = smoothstep(u_MinDistance, u_MaxDistance, abs(-positionViewSpace.z - focusPoint));

  fragColor = mix(inFocusColor, outFocusColor, blur);

  gColor = fragColor;

}