# OpenGL Study

A repository for OpenGL studying.

# 相关资料
## 教程地址：

  - 原教程地址：https://learnopengl.com/
  
  - 中文教程地址：https://learnopengl-cn.github.io/

## 参考网址：

  - OpenGL API doc： http://docs.gl/
  
  - GLFW： http://www.glfw.org/index.html
  
  - GLEW： http://glew.sourceforge.net/
  
  - stb: https://github.com/nothings/stb
  
  - assimp：https://github.com/assimp/assimp
  
  - OpenGL tutorial video：https://www.youtube.com/watch?v=W3gAzLwfIP0&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2

  - Xcode GLFW & GLEW 环境配置：https://blog.csdn.net/LongZh_CN/article/details/55001345

# 目录

1. [你好，三角形](https://github.com/yangruihan/OpenGL_study/tree/master/OpenGL_study/src/test/test1)

2. [着色器](https://github.com/yangruihan/OpenGL_study/tree/master/OpenGL_study/src/test/test2)

# 笔记
## 图形渲染管线（Graphics Pipeline）

指的是一堆原始图形数据途径一个输送管道，期间经过各种变化处理，最终出现在屏幕的过程

可以被划分为两个主要部分：

1. 3D坐标 -> 2D坐标
2. 2D坐标 -> 有颜色的像素

流程如下图所示：

![](README/1.png)

![](README/2.png)

## 标准化设备坐标（Normalized Device Coordinates，NDC）

![](README/3.png)
