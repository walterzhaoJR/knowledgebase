#├── CMakeLists.txt
#├── subbinary
#│   ├── CMakeLists.txt
#│   └── main.cpp
#├── sublibrary1
#│   ├── CMakeLists.txt
#│   ├── include
#│   │   └── sublib1
#│   │       └── sublib1.h
#│   └── src
#│       └── sublib1.cpp
#└── sublibrary2
#    ├── CMakeLists.txt
#    └── include
#        └── sublib2
#            └── sublib2.h


#sublibrary1 - 一个静态库
#sublibrary2 - 只有头文件的库
#subbinary - 一个可执行文件


cmake_minimum_required(VERSION 2.8.12)
project(subprojects)

# Add sub directories
add_subdirectory(sublibrary1)
#add_subdirectory(sublibrary2)
add_subdirectory(subbinary)