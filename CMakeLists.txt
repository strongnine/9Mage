# requirement of cmake version
# 设置 cmake 要求的最低版本号：
cmake_minimum_required(VERSION 3.5)

# project name
# 这个指令隐式的定义了两个 cmake 变量: 
#     1. <projectname>_BINARY_DIR - 构建路径
#     2. <projectname>_SOURCE_DIR - 工程路径 (CMakeLists.txt 所在路径)
# PROJECT_BINARY_DIR 和 PROJECT_SOURCE_DIR变量
PROJECT(9Mage)

# set the directory of executable files
# SET 指令的语法: 
#     SET(VAR [VALUE] [CACHE TYPE DOCSTRING [FORCE]])
# 显式的定义变量 VAR 为 [VALUE]
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${9Mage_SOURCE_DIR}/bin)
# SET(CMAKE_C_COMPILER "/usr/bin/gcc")
# SET(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_OSX_ARCHITECTURES arm64)
# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lstdc++")

# find required opencv
# find_package 这个指令以被用来在系统中自动查找配置构建工程所需的程序库
# 当它找到 OpenCV 程序库之后，就会帮助我们预定义几个变量: 
#     1. OpenCV_FOUND        - 是否找到 OpenCV
#     2. OpenCV_INCLUDE_DIRS - OpenCV 的头文件目录
#     3. OpenCV_LIBRARY_DIRS - OpenCV 的库文件目录
#     4. OpenCV_LIBRARIES    - OpenCV 的所有库文件列表
find_package(OpenCV REQUIRED)

# name of executable file and path of source file
# 可执行文件的名字，和对应源码的名字
# !!! 这个命令一定要在 TARGET_LINK_LIBRARIES 之前使用
add_executable(9Mage src/9Mage.cpp src/methods/secedct.cpp)

# directory of opencv headers
# 设置包含的头文件的路径
include_directories(${OpenCV_INCLUDE_DIRS})

# directory of opencv library
link_directories(${OpenCV_LIBRARY_DIRS})

# opencv libraries
# 设置库文件的路径
target_link_libraries(9Mage ${OpenCV_LIBS})

# 在 VSCode 上使用 clangd 插件，遇到头文件 include 时找不到，可以执行命令: 
#     cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1
# 会自动生成 compile_commands.json 文件，如果 cmake 可以成功执行并生成最终项目文件，
# 就可以解决 include 找不到的问题
