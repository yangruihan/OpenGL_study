#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;

out vec3 o_Normal;
out vec3 o_FragPos;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
    gl_Position = u_Proj * u_View * u_Model * position;
    o_Normal = mat3(transpose(inverse(u_Model))) * normal.xyz;
    o_FragPos = vec3(u_Model * position);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_LightColor;
uniform vec4 u_ObjColor;
uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;

in vec3 o_Normal;
in vec3 o_FragPos;

void main()
{
    // ambient
    float ambient_strength = 0.1f;
    vec3 ambient = ambient_strength * u_LightColor.xyz;


    // normal && light dir
    vec3 norm = normalize(o_Normal);
    vec3 light_dir = normalize(u_LightPos - o_FragPos);

    // diffuse
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * u_LightColor.xyz;

    // specular
    float specular_strength = 0.5f;
    vec3 view_dir = normalize(u_ViewPos - o_FragPos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
    vec3 specular = specular_strength * spec * u_LightColor.xyz;

    // result
    vec3 result = (ambient + diffuse + specular) * u_ObjColor.xyz;
    color = vec4(result, 1.0);
}
