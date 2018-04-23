#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

uniform mat4 u_MVP;

out vec4 o_Color;

void main()
{
    gl_Position = u_MVP * position;
    o_Color = color;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;

in vec4 o_Color;

void main()
{
    color = o_Color;
}
