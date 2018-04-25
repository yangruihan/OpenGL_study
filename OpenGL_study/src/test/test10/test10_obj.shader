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

/**
 * 材质结构体
 */
struct Material {
    sampler2D   diffuse;    // 环境光
    sampler2D   specular;   // 漫反射光
    float       shininess;  // 镜面光
};

/**
 * 方向光结构体
 */
struct DirLight {
    vec3    direction;

    // --- 基本参数 --- //
    vec3    ambient;    // 环境光
    vec3    diffuse;    // 漫反射光
    vec3    specular;   // 镜面光
};

/**
 * 点光源结构体
 */
struct PointLight {
    vec3    position;

    // --- 基本参数 --- //
    vec3    ambient;    // 环境光
    vec3    diffuse;    // 漫反射光
    vec3    specular;   // 镜面光

    // --- 点光源参数 --- 参考 https://learnopengl-cn.github.io/02%20Lighting/05%20Light%20casters/ //
    float   constant;   // 常量
    float   linear;     // 一次项
    float   quadratic;  // 二次项
};

/**
 * 聚光结构体
 */
struct SpotLight {
    vec3    position;
    vec3    direction;

    // --- 基本参数 --- //
    vec3    ambient;    // 环境光
    vec3    diffuse;    // 漫反射光
    vec3    specular;   // 镜面光

    // --- 聚光灯参数 --- //
    float   cut_off;
    float   outer_cut_off;
};


#define NR_POINT_LIGHTS 4

layout(location = 0) out vec4 color;


// --- uniform ---//
uniform vec3 u_ViewPos;
uniform Material u_Material;

uniform DirLight u_DirLight;
uniform PointLight u_PointLights[NR_POINT_LIGHTS];
uniform SpotLight u_SpotLight;

uniform float u_DistanceRate;


// --- in parameters ---//
in vec3 o_Normal;
in vec3 o_FragPos;
in vec2 o_TextureCoords;


// --- function definition --- //
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 view_dir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 view_dir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 view_dir);


void main()
{
    // normal && light dir
    vec3 normal = normalize(o_Normal);
    vec3 view_dir = normalize(u_ViewPos - o_FragPos);
    vec3 result = vec3(0.0f);

    // 1. direction light
    result += CalcDirLight(u_DirLight, normal, view_dir);

    // 2. point light
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(u_PointLights[i], normal, view_dir);

    result += CalcSpotLight(u_SpotLight, normal, view_dir);

    color = vec4(result, 1.0f);
}


/**
 * 计算方向光函数
 */
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 view_dir)
{
    vec3 light_dir = normalize(-light.direction);

    // ambient
    vec3 ambient = light.ambient * vec3(texture(u_Material.diffuse, o_TextureCoords));

    // diffuse
    float diff   = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(u_Material.diffuse, o_TextureCoords)));

    // specular
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec       = pow(max(dot(view_dir, reflect_dir), 0.0), u_Material.shininess);
    vec3 specular    = light.specular * (spec * vec3(texture(u_Material.specular, o_TextureCoords)));

    return (ambient + diffuse + specular);
}


/**
 * 计算点光源函数
 */
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 view_dir)
{
    vec3 light_dir = normalize(light.position - o_FragPos);

    float distance = length(vec3(light.position) - o_FragPos) / u_DistanceRate;
    float attenuation = 1.0 /
                (light.constant + light.linear * distance + light.quadratic * (distance * distance));

        // ambient
    vec3 ambient = light.ambient * vec3(texture(u_Material.diffuse, o_TextureCoords));

    // diffuse
    float diff   = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(u_Material.diffuse, o_TextureCoords)));

    // specular
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec       = pow(max(dot(view_dir, reflect_dir), 0.0), u_Material.shininess);
    vec3 specular    = light.specular * (spec * vec3(texture(u_Material.specular, o_TextureCoords)));

    return (ambient + diffuse + specular) * attenuation;
}


/**
 * 计算聚光函数
 */
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 view_dir)
{
    vec3 light_dir = normalize(light.position - o_FragPos);

    // ambient
    vec3 ambient = light.ambient * vec3(texture(u_Material.diffuse, o_TextureCoords));

    // diffuse
    float diff   = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(u_Material.diffuse, o_TextureCoords)));

    // specular
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec       = pow(max(dot(view_dir, reflect_dir), 0.0), u_Material.shininess);
    vec3 specular    = light.specular * (spec * vec3(texture(u_Material.specular, o_TextureCoords)));

    float theta     = dot(light_dir, normalize(-light.direction));
    float epsilon   = light.cut_off - light.outer_cut_off;
    float intensity = clamp((theta - light.outer_cut_off) / epsilon, 0.0f, 1.0f);

    diffuse  *= intensity;
    specular *= intensity;

    return (ambient + diffuse + specular);
}
