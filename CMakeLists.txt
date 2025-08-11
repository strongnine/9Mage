# requirement of cmake version
# 设置 cmake 要求的最低版本号：
cmake_minimum_required(VERSION 3.5)
PROJECT(9Mage
    LANGUAGES CXX
    VERSION 1.0.0
    DESCRIPTION "Set of Image processing algorithms. ")

find_package(OpenCV REQUIRED)

# find required opencv
# find_package 这个指令以被用来在系统中自动查找配置构建工程所需的程序库
# 当它找到 OpenCV 程序库之后，就会帮助我们预定义几个变量:
# 1. OpenCV_FOUND        - 是否找到 OpenCV
# 2. OpenCV_INCLUDE_DIRS - OpenCV 的头文件目录
# 3. OpenCV_LIBRARY_DIRS - OpenCV 的库文件目录
# 4. OpenCV_LIBRARIES    - OpenCV 的所有库文件列表
find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)
