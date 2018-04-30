#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 texture_coords;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

out vec3 o_Normal;
out vec3 o_Position;

void main()
{
    gl_Position = u_Proj * u_View * u_Model * position;
    o_Position = vec3(u_Model * position);
    o_Normal = mat3(transpose(inverse(u_Model))) * normal.xyz;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec3 u_CamPos;
uniform samplerCube u_Texture;

in vec3 o_Normal;
in vec3 o_Position;

void main()
{
    vec3 i = normalize(o_Position - u_CamPos);
    vec3 r = refract(i, normalize(o_Normal), 1.00 / 1.52);
    color = vec4(texture(u_Texture, vec3(r.x, -r.y, r.z)).rgb, 1.0f);
}
