# 摄像机

## 参考教程

- 英文原版：http://learnopengl.com/#!Getting-started/Camera

- 中文版：https://learnopengl-cn.github.io/01%20Getting%20started/09%20Camera/


## 效果

![](../../../../README/test5_camera.gif)

## 关键字
- 观察空间（View Space）

    ![](../../../../README/test5_view_space.png)

- Look At 矩阵

    ![](../../../../README/test5_look_at_matrix.png)

    其中：

    - <font color="red">R</font> 是**摄像机右向量**
    - <font color="green">U</font> 是**摄像机上向量**
    - <font color="blue">D</font> 是**摄像机方向**
    - <font color="purple">P</font> 是**摄像机位置**

- [欧拉角](https://zh.wikipedia.org/zh-hans/%E6%AC%A7%E6%8B%89%E8%A7%92)

    - 俯仰角（Pitch）

    - 偏航角（Yaw）

    - 滚转角（Row）

## 说明
- [未封装成 Camera 类对应代码](https://github.com/yangruihan/OpenGL_study/tree/master/OpenGL_study/src/test/test5/test5.cpp)

- [封装 Camera 类对应代码](https://github.com/yangruihan/OpenGL_study/tree/master/OpenGL_study/src/test/test5/test5_use_camera.cpp)
