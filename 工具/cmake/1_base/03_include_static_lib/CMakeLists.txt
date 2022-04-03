#├── CMakeLists.txt
#├── include
#│   └── static
#│       └── Hello.h
#└── src
#    ├── Hello.cpp
#    └── main.cpp

cmake_minimum_required(VERSION 2.8.12)
project(hello_library)

############################################################
# Create a library
############################################################
# 库的源文件hello.cpp生成静态库hello_library
add_library(hello_library STATIC src/hello.cpp)

# target_include_directories为一个目标（可能是一个库library也可能是可执行文件）添加头文件路径
target_include_directories(hello_library PUBLIC ${PROJECT_SOURCE_DIR}/include)


############################################################
# Create an executable
############################################################
# Add an executable with the above sources
# 指定用哪个源文件生成可执行文件

add_executable(hello src/main.cpp)


# 链接可执行文件和静态库
target_link_libraries(hello PRIVATE hello_library)