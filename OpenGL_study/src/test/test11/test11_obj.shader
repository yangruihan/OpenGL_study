#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
    TexCoords = aTexCoords;    
    gl_Position = u_Proj * u_View * u_Model * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_normal1;
    sampler2D texture_height1;
};

out vec4 FragColor;

in vec2 TexCoords;

uniform Material u_Material;

void main()
{    
    FragColor = texture(u_Material.texture_diffuse1, TexCoords);
}