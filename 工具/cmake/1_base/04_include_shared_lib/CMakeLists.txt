#├── CMakeLists.txt
#├── include
#│   └── shared
#│       └── Hello.h
#└── src
#    ├── Hello.cpp
#    └── main.cpp

cmake_minimum_required(VERSION 2.8.12)
project(hello_library)

############################################################
# Create a library
############################################################
#根据Hello.cpp生成动态库

add_library(hello_library SHARED src/hello.cpp)

#给动态库hello_library起一个别的名字hello::library
add_library(hello::library ALIAS hello_library)

target_include_directories(hello_library PUBLIC ${PROJECT_SOURCE_DIR}/include)

############################################################
# Create an executable
############################################################
# 根据main.cpp生成可执行文件

add_executable(hello_binary src/main.cpp)

# 链接库和可执行文件，使用的是这个库的别名
target_link_libraries(hello_binary PRIVATE hello::library)

# 以下是不使用别名的方式
#target_link_libraries(hello_binary PRIVATE hello_library)