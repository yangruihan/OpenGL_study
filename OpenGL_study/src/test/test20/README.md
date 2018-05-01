# 高级光照

- 英文原版：https://learnopengl.com/Advanced-Lighting/Advanced-Lighting

- 中文版：https://learnopengl-cn.github.io/05%20Advanced%20Lighting/01%20Advanced%20Lighting/

# Blinn-Phong 模型
为了解决反射向量与观察向量夹角大于90°时，造成的视觉问题，引入 [Blinn-Phong](https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_shading_model) 模型，与 [Phong](https://en.wikipedia.org/wiki/Phong_shading) 模型最大的区别在于，引入了 **半程向量（halfway-vector）** 参与计算。

> **Halfway vector** is a unit vector exactly halfway between the view direction and the light direction. 

翻译过来就是： **半程向量** 是一个单位向量， 它在视线方向和光照方向正中间。

可以发现，观察者无论从哪个方向观看，半程向量和表面法线夹角都不会超过90°，可以产生更可信的结果。

# 实现
GLSL代码：

```[GLSL]
vec3 lightDir   = normalize(lightPos - FragPos);
vec3 viewDir    = normalize(viewPos - FragPos);
vec3 halfwayDir = normalize(lightDir + viewDir);
```

```[GLSL]
float spec    = pow(max(dot(normal, halfwayDir), 0.0), shininess);
vec3 specular = lightColor * spec;
```
