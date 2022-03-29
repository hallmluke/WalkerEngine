#version 450 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 GeomPos[];
in vec3 NormalGeom[];
in vec2 TexCoordsGeom[];
in vec4 GeomPosLightSpace[];

out vec3 FragPos;
out vec3 NormalFrag;
out vec2 TexCoordsFrag;
out vec4 FragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main() {
	
	const vec3 p1 = GeomPos[1] - GeomPos[0];
	const vec3 p2 = GeomPos[2] - GeomPos[0];
	const vec3 p = abs(cross(p1, p2)); 
	for(uint i = 0; i < 3; ++i){
		FragPos = GeomPos[i];
		NormalFrag = NormalGeom[i];
		TexCoordsFrag = TexCoordsGeom[i];
		FragPosLightSpace = GeomPosLightSpace[i];
		if(p.z > p.x && p.z > p.y){
			gl_Position = projection * view * vec4(FragPos.x, FragPos.y, FragPos.z, 1);
		} else if (p.x > p.y && p.x > p.z){
			gl_Position = projection * view * vec4(FragPos.z, FragPos.y, FragPos.x, 1);
		} else {
			gl_Position = projection * view * vec4(FragPos.x, FragPos.z, FragPos.y, 1);
		}
		EmitVertex();
	}
    EndPrimitive();
}