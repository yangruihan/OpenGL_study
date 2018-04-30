#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 texture_coords;

uniform mat4 u_Model;

layout(std140) uniform u_Matrices
{
    uniform mat4 u_Proj;
    uniform mat4 u_View;
};

out VS_OUT {
    vec3 o_Normal;
} vs_out;

void main()
{
    gl_Position = u_Proj * u_View * u_Model * position;
    mat3 normal_mat = mat3(transpose(inverse(u_View * u_Model)));
    vs_out.o_Normal = normalize(vec3(u_Proj * vec4(normal_mat * normal.xyz, 0.0f)));
}


#------------------------------------------------------------------------------------------
#shader geometry
#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 o_Normal;
} gs_in[];

const float MAGNITUDE = 50;

void GenerateLine(int index)
{
    gl_Position = gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].o_Normal, 0.0) * MAGNITUDE;
    EmitVertex();
    EndPrimitive();
}

void main()
{
    GenerateLine(0); // 第一个顶点法线
    GenerateLine(1); // 第二个顶点法线
    GenerateLine(2); // 第三个顶点法线
}


#------------------------------------------------------------------------------------------
#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

void main()
{
    color = vec4(255.0 / 255.0, 99.0 / 255.0, 71.0 / 255.0, 1.0);
}
