#├── CMakeLists.txt
#├── main.cpp

# 构建类型 build-type

cmake_minimum_required(VERSION 2.8.12)

# 如果没有指定则设置默认编译方式

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  # 在命令行中输出message里的信息
  message("Setting build type to 'RelWithDebInfo' as none was specified.")

  # 不管CACHE里有没有设置过CMAKE_BUILD_TYPE这个变量，都强制赋值这个值为RelWithDebInfo
  message("CMAKE_BUILD_TYPE:"${CMAKE_BUILD_TYPE})
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)
  message("CMAKE_BUILD_TYPE:"${CMAKE_BUILD_TYPE})

  # 当使用cmake-gui的时候，设置构建级别的四个可选项
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()


project(build_type)
add_executable(cmake_examples_build_type main.cpp)