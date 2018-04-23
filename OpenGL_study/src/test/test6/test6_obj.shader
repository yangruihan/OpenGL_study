#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * position;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_LightColor;
uniform vec4 u_ObjColor;

void main()
{
    float ambient_strength = 0.1f;
    vec3 ambient = ambient_strength * u_LightColor.xyz;
    vec3 result = ambient * u_ObjColor.xyz;
    color = vec4(result, 1.0);
}
