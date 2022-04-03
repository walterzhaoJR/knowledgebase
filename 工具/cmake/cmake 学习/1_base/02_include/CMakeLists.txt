#├── CMakeLists.txt
#├── include
#│   └── Hello.h
#└── src
#    ├── Hello.cpp
#    └── main.cpp


cmake_minimum_required(VERSION 2.8.12)
project(hello_headers)


#创建一个变量，名字叫SOURCE。它包含了所有的cpp文件
set(SOURCES
src/hello.cpp
src/main.cpp
)

add_executable(hello_headers ${SOURCES})
# 等价于 add_executable(hello_headers src/Hello.cpp src/main.cpp)

target_include_directories(hello_headers PRIVATE ${PROJECT_SOURCE_DIR}/include)

#PROJECT_SOURCE_DIR指工程顶层目录
#PROJECT_BINARY_DIR指编译目录
#PRIVATE指定了库的范围