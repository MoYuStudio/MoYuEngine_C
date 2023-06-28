# MoYuEngine_C

MoYuEngine_C 是一个使用 C 语言开发的简单游戏引擎。它基于 Raylib 游戏开发库，并提供了一些封装和工具函数，方便快速开发 2D 游戏。

## 功能特性

- 封装了一些常用的游戏开发功能，如图形绘制、输入处理、碰撞检测等。
- 提供了简单的场景管理和游戏循环逻辑。
- 支持资源管理，包括纹理、声音和精灵等。
- 集成了调试工具和性能监控功能。

## 依赖

- [Raylib](https://www.raylib.com/) - 开源的简单易用的游戏开发库。

## 安装和运行

1. 克隆或下载本仓库。

2. 安装 Raylib 游戏开发库。请参考 Raylib 官方网站（https://www.raylib.com/）上的安装说明。

3. 编译和运行项目：

    ```bash
    $ gcc main.c -o MoYuEngine_C -lraylib
    $ ./MoYuEngine_C
    ```

## 示例项目

本仓库中包含了一个简单的示例项目，展示了如何使用 MoYuEngine_C 开发一个基本的 2D 游戏。你可以在 `examples` 文件夹中找到该示例项目。运行示例项目前，请确保已经安装了 Raylib 游戏开
