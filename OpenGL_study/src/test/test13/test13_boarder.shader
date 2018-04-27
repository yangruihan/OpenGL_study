#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
    gl_Position = u_Proj * u_View * u_Model * position;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

void main()
{
    color = vec4(255.0 / 255.0, 99.0 / 255.0, 71.0 / 255.0, 1.0);
}
