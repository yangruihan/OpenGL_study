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

void main()
{
    gl_Position = u_Proj * u_View * u_Model * position;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

void main()
{
    color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
