[![ID](https://img.shields.io/badge/ID-strongnine-orange)](https://github.com/strongnine) [![ID](https://img.shields.io/badge/website-9Mage-orange)](https://strongnine.github.io/9Docs/dev/) [![ID](https://img.shields.io/badge/version-v0.0.2-orange)](https://strongnine.github.io/9Docs/dev/)

# 9Mage

笔记本：MacBook Air M1 2020

系统：macOS Sequoia15.0

环境：OpenCV、CMake

## 用法

### 如何编译

```shell
$ git clone git@github.com:strongnine/9Mage.git
$ cd 9Mage
$ mkdir build && cd build
$ cmake ..
$ make
```

### 如何使用

在终端中运行 `9Mage -h` 或者 `9Mage --help` 可以查看使用方法。

通过 `-m <method>` 或者 `--method <method>` 可以指定图片处理的方法。

当前支持的方法有：

1. `SECEDCT`：
2. `USM`：

通过指定 `--show=off` 可以关闭处理结果预览。
