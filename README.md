[![ID](https://img.shields.io/badge/ID-strongnine-orange)](https://github.com/strongnine) [![ID](https://img.shields.io/badge/website-9Mage-orange)](https://github.com/strongnine/9Mage) [![ID](https://img.shields.io/badge/version-v0.1.0-orange)](https://strongnine.github.io/9Docs/dev/)

# 9Mage

笔记本：MacBook Air M1 2020

系统：macOS Sequoia 15.0

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

进入 `9Mage` 目录，使用如下命令：

```shell
# 使用方法 USM 处理图片 ./data/tiananmen.png 并将结果保存为 ./data/USM.png
$ ./bin/9Mage ./data/tiananmen.png ./data/USM.png --method USM
# 如果想要显示结果图片可以加上 -s 参数（弹出窗口后按 `q` 可以关闭窗口）
$ ./bin/9Mage ./data/tiananmen.png ./data/USM.png --method USM -s
```

在终端中运行  `./bin/9Mage --help` 可以查看使用方法。

通过 `-m <method>` 或者 `--method <method>` 指定图片处理的方法。

当前支持的方法有：

1. `SEGCE`：通过计算图像的空间熵，得到一个可以拉伸像素值的映射函数，从而提高图像的对比度。参考：[基于空间熵的全局对比度增强（SEGCE）](https://strongnine.github.io/9Docs/dev/CV/contrast/#%E5%9F%BA%E4%BA%8E%E7%A9%BA%E9%97%B4%E7%86%B5%E7%9A%84%E5%85%A8%E5%B1%80%E5%AF%B9%E6%AF%94%E5%BA%A6%E5%A2%9E%E5%BC%BA%EF%BC%88SEGCE%EF%BC%89)。
2. `USM`：
3. `AGC`：自动 Gamma 矫正（Auto Gamma Correction）

通过指定 `-h` 或者 `--show` 可以把处理结果显示出来。

