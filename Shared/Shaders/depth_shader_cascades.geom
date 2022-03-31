#version 460 core
    
layout(triangles, invocations = 5) in;
layout(triangle_strip, max_vertices = 3) out;
    
layout (std140, binding = 0) uniform LightSpaceMatrices
{
    mat4 lightSpaceMatrices[16];
};

//in vec2 TexCoordsGeom[];
//out vec2 TexCoords;
    
void main()
{          
    for (int i = 0; i < 3; ++i)
    {
        gl_Position = 
            lightSpaceMatrices[gl_InvocationID] * gl_in[i].gl_Position;
        gl_Layer = gl_InvocationID;
        //TexCoords = TexCoordsGeom[i];
        EmitVertex();
    }
    EndPrimitive();
}  