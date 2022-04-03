


## 01_hello_cmake
### cmake_minimum_required
* cmake的最小版本

### project
* 项目名称
* 会生成一个变量：PROJECT_NAME

### add_executable
* 生成可执行文件
* add_executable()命令指定某些源文件生成可执行文件，本例中是main.cpp。 add_executable（）函数的第一个参数是可执行文件名，第二个参数是要编译的源文件列表。

### 构建位置
* 变量CMAKE_BINARY_DIR指向 cmake命令的根文件夹，所有二进制文件在这个文件夹里产生。
* 使用外部构建，我们可以创建一个可以位于文件系统上任何位置的构建文件夹。 所有临时构建和目标文件都位于此目录中，以保持源代码树的整洁。(外部构建)
```linux
mkdir build
cd build/
cmake ..
```
* 如果直接在源码目录cmake . 就是内部构建

## 02_include
### 可用变量
* CMake语法指定了许多变量，可用于帮助您在项目或源代码树中找到有用的目录。 其中一些包括：
  * CMAKE_SOURCE_DIR	根源代码目录，工程顶层目录。暂认为就是PROJECT_SOURCE_DIR
  * CMAKE_CURRENT_SOURCE_DIR	当前处理的 CMakeLists.txt 所在的路径
  * PROJECT_SOURCE_DIR	工程顶层目录
  * CMAKE_BINARY_DIR	运行cmake的目录。外部构建时就是build目录
  * 当前所在build目录
  * PROJECT_BINARY_DIR	暂认为就是CMAKE_BINARY_DIR
* 这些变量不仅可以在CMakeLists中使用，同样可以在源代码.cpp中使用。

### 包含头文件
* 当有其他需要包含的文件夹（文件夹里有头文件）时，可以使用以下命令使编译器知道它们： target_include_directories（）。 编译此目标时，这将使用-I标志将这些目录添加到编译器中，例如 -I /目录/路径。
* PRIVATE 标识符指定包含的范围。

### 详细信息
* 在前面的示例中，运行make命令时，输出仅显示构建状态。 要查看用于调试目的的完整输出，可以在运行make时添加VERBOSE = 1标志。