#├── CMakeLists.txt
#├── main.cpp

cmake_minimum_required(VERSION 2.8.12)
project(third_party_include)

# 使用库文件系统和系统查找boost install
find_package(Boost 1.46.1 REQUIRED COMPONENTS filesystem system)

# 这是第三方库，而不是自己生成的静态动态库

if(Boost_FOUND)
    message ("boost found")
else()
    message (FATAL_ERROR "Cannot find Boost")
endif()


add_executable(third_party_include main.cpp)

# link against the boost libraries
target_link_libraries(third_party_include PRIVATE Boost::filesystem)


# 注意本示例要将boost安装的在默认位置，要在/usr/share/cmake/Modules目录下有FindXXX.cmake文件