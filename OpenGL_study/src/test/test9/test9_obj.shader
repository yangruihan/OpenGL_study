#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 texture_coords;

out vec3 o_Normal;
out vec3 o_FragPos;
out vec2 o_TextureCoords;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
    gl_Position = u_Proj * u_View * u_Model * position;
    o_Normal = mat3(transpose(inverse(u_Model))) * normal.xyz;
    o_FragPos = vec3(u_Model * position);
    o_TextureCoords = texture_coords;
}

#shader fragment
#version 330 core

struct Material {
    sampler2D   diffuse;
    sampler2D   specular;
    float       shininess;
};

struct Light {
    vec4    direction;

    vec3    ambient;
    vec3    diffuse;
    vec3    specular;

    float constant;
    float linear;
    float quadratic;
};

layout(location = 0) out vec4 color;

uniform vec3 u_ViewPos;
uniform Material u_Material;
uniform Light u_Light;
uniform float u_DistanceRate;

in vec3 o_Normal;
in vec3 o_FragPos;
in vec2 o_TextureCoords;

void main()
{
    // normal && light dir
    vec3 norm = normalize(o_Normal);
    vec3 light_dir;
    float attenuation = 1.0;

    // w 分量为0，表示方向向量，w 分量为1，表示位置向量
    if (u_Light.direction.w == 0.0f)
    {
        light_dir = normalize(-vec3(u_Light.direction.xyz));
    }
    else
    {
        light_dir = normalize(vec3(u_Light.direction.xyz) - o_FragPos);

        float distance = length(vec3(u_Light.direction) - o_FragPos) / u_DistanceRate;
        attenuation = 1.0 /
            (u_Light.constant + u_Light.linear * distance + u_Light.quadratic * (distance * distance));
    }

    // ambient
    vec3 ambient = u_Light.ambient * vec3(texture(u_Material.diffuse, o_TextureCoords));

    // diffuse
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = u_Light.diffuse * (diff * vec3(texture(u_Material.diffuse, o_TextureCoords)));

    // specular
    vec3 view_dir = normalize(u_ViewPos - o_FragPos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), u_Material.shininess);
    vec3 specular = u_Light.specular * (spec * vec3(texture(u_Material.specular, o_TextureCoords)));

    // result
    vec3 result = (ambient + diffuse + specular) * attenuation;
    color = vec4(result, 1.0);
}
