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

const float offset = 1.0 / 300.0;

void main()
{
    // 反色
    // color = vec4(vec3(1.0f - texture(u_Texture, o_TextureCoord)), 1.0f);

    // 灰度
    // color = texture(u_Texture, o_TextureCoord);
    // float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
    // color = vec4(vec3(average), 1.0);

    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // 左上
        vec2( 0.0f,    offset), // 正上
        vec2( offset,  offset), // 右上
        vec2(-offset,  0.0f),   // 左
        vec2( 0.0f,    0.0f),   // 中
        vec2( offset,  0.0f),   // 右
        vec2(-offset, -offset), // 左下
        vec2( 0.0f,   -offset), // 正下
        vec2( offset, -offset)  // 右下
    );

    // 锐化
    // float kernel[9] = float[](
    //     -1, -1, -1,
    //     -1,  9, -1,
    //     -1, -1, -1
    // );

    // 模糊
    // float kernel[9] = float[](
    //     1.0 / 16, 2.0 / 16, 1.0 / 16,
    //     2.0 / 16, 4.0 / 16, 2.0 / 16,
    //     1.0 / 16, 2.0 / 16, 1.0 / 16  
    // );

    // 边缘检测
    float kernel[9] = float[](
         1,  1,  1,
         1, -8,  1,
         1,  1,  1
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(u_Texture, o_TextureCoord.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    color = vec4(col, 1.0);
}
