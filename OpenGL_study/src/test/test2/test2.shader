#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 u_MVP;

out vec4 o_Pos;

void main()
{
    vec4 pos = u_MVP * position;
    gl_Position = vec4(pos.x, -pos.y, pos.zw);
    o_Pos = gl_Position;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;

in vec4 o_Pos;

void main()
{
    color = o_Pos;
}
