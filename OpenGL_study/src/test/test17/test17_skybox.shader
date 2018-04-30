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

out vec3 o_TextureCoord;

void main()
{
    gl_Position = (u_Proj * u_View * u_Model * position).xyww;
    o_TextureCoord = position.xyz;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform samplerCube u_Texture;

in vec3 o_TextureCoord;

void main()
{
    color = texture(u_Texture, vec3(o_TextureCoord.x, -o_TextureCoord.y, o_TextureCoord.z));
}
