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
    vec3    direction;

    vec3    ambient;
    vec3    diffuse;
    vec3    specular;
};

layout(location = 0) out vec4 color;

uniform vec3 u_ViewPos;
uniform Material u_Material;
uniform Light u_Light;

in vec3 o_Normal;
in vec3 o_FragPos;
in vec2 o_TextureCoords;

void main()
{
    // ambient
    vec3 ambient = u_Light.ambient * vec3(texture(u_Material.diffuse, o_TextureCoords));

    // normal && light dir
    vec3 norm = normalize(o_Normal);
    vec3 light_dir = normalize(-u_Light.direction);

    // diffuse
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = u_Light.diffuse * (diff * vec3(texture(u_Material.diffuse, o_TextureCoords)));

    // specular
    vec3 view_dir = normalize(u_ViewPos - o_FragPos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), u_Material.shininess);
    vec3 specular = u_Light.specular * (spec * vec3(texture(u_Material.specular, o_TextureCoords)));

    // result
    vec3 result = (ambient + diffuse + specular);
    color = vec4(result, 1.0);
}
