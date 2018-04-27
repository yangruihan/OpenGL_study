#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texture_coords;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

out vec2 o_TextureCoord;

void main()
{
    gl_Position = u_Proj * u_View * u_Model * position;
    o_TextureCoord = texture_coords;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform sampler2D u_Texture;

in vec2 o_TextureCoord;

void main()
{
    vec4 result = texture(u_Texture, o_TextureCoord);
    if (result.w < 0.1)
        discard;
    color = result;
}
