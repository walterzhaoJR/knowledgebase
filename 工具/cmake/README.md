
## info
* [引用](https://sfumecjf.github.io/cmake-examples-Chinese/)
* [github](https://github.com/ttroy50/cmake-examples)

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

## 02_include_head
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

* 注意：
  * 在SOURCES变量中设置特定文件名的另一种方法是使用GLOB命令使用通配符模式匹配来查找文件。file(GLOB SOURCES `src/*.cpp`)使用`*`这个通配符，表示所有.cpp结尾的文件都会包含到这个SOURCES变量。
  *  对于modern CMake，不建议对源文件使用变量。 不建议使用glob。

  * 相反，通常直接在add_xxx函数中声明源文件。

  * 这对于glob命令尤其重要，如果添加新的源文件，这些命令可能不会始终为您显示正确的结果。在CMake中指定源文件的最佳方法是明确列出它们。


### 详细信息
* 在前面的示例中，运行make命令时，输出仅显示构建状态。 要查看用于调试目的的完整输出，可以在运行make时添加VERBOSE = 1标志。

## 03_include_static_lib
### 创建静态库
* add_library（）函数用于从某些源文件创建一个库，默认生成在构建文件夹,在add_library调用中包含了源文件，用于创建名称为libhello_library.a的静态库。
```linux
add_library(hello_library STATIC src/Hello.cpp
)
```

### 添加头文件所在目录
* 使用target_include_directories（）添加了一个目录，这个目录是库所包含的头文件的目录，并设置库属性为PUBLIC。使用这个函数后，这个目录会在以下情况被调用
  * 编译这个库的时候,因为这个库hello_library由Hello.cpp生成，Hello.cpp中函数的定义在Hello.h中，Hello.h在这个include目录下，所以显然编译这个库的时候，这个目录会用到
  * 编译链接到这个库hello_library的任何其他目标（库或者可执行文件）
```linux
target_include_directories(hello_library
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)
```
* 这里注意区别private pubic interface的范围：
  * PRIVATE - 目录被添加到目标（库）的包含路径中。
  * INTERFACE - 目录没有被添加到目标（库）的包含路径中，而是链接了这个库的其他目标（库或者可执行程序）包含路径中
  * PUBLIC - 目录既被添加到目标（库）的包含路径中，同时添加到了链接了这个库的其他目标（库或者可执行程序）的包含路径中
* 也就是说，根据库是否包含这个路径，以及调用了这个库的其他目标是否包含这个路径，可以分为三种范围。这里有个使用的建议：
  * 对于公共的头文件，最好在include文件夹下建立子目录
  * 传递给函数target_include_directories()的目录，应该是所有包含目录的根目录，然后在这个根目录下建立不同的文件夹，分别写头文件。
  * 这样使用的时候，不需要写${PROJECT_SOURCE_DIR}/include，而是直接选择对应的文件夹里对应头文件。下面是例子：#include "static/Hello.h"而不是#include "Hello.h"使用此方法意味着在项目中使用多个库时，头文件名冲突的可能性较小。

### 链接库
* 创建将使用这个库的可执行文件时，必须告知编译器需要用到这个库。 可以使用target_link_library（）函数完成此操作。add_executable()连接源文件，target_link_libraries()连接库文件。
```linux
add_executable(hello_binary src/main.cpp)

target_link_libraries(hello_binary PRIVATE hello_library)
```

* 这告诉CMake在链接期间将hello_library链接到hello_binary可执行文件。 同时，这个被链接的库如果有INTERFACE或者PUBLIC属性的包含目录，那么，这个包含目录也会被传递给这个可执行文件。

* An example of this being called by the compiler is
```linux
/usr/bin/c++ CMakeFiles/hello_binary.dir/src/main.cpp.o -o hello_binary -rdynamic libhello_library.a
```

* 官方英文文档关于target_link_libraries(hello_binary PRIVATE hello_library )这个命令中的范围关键字，private，public以及interface的解释：
  * public是说，你的这个工程如果被link了，那你的target_link_libraries指定的lib也会被链。
  * private是说，你link的这些libs不会被暴露出去。
  * 比如你的工程B是个dll，public连接了C, D 这个时候你的A.exe要链接B，那么它也会链接C和D 如果B是private链接了C, D 那么A链B的时候，不会链C和D。
  * 那么，A.exe链接B的时候，其实也有public和private的选项，但是因为没有其他东西链接A，所以不起作用。 这个主要是针对其它工程链自己的设置。
  * 对于hello_binary，它不是库，所以不会被链接。直接private自己用这个库就行。

## 04_include_shared_lib
### 创建动态库
* add_library（）函数用于从某些源文件创建一个动态库，默认生成在构建文件夹。 写法如下:
```linux
add_library(hello_library SHARED
    src/Hello.cpp
)
```
* 在add_library调用中包含了源文件，用于创建名称为libhello_library.so的动态库。如前面的示例所述，将源文件直接传递给add_library调用，这是modern CMake的建议。（而不是先把Hello.cpp赋给一个变量）

### 创建别名目标
* 顾名思义，别名目标是在只读上下文中可以代替真实目标名称的替代名称。
```linux
add_library(hello::library ALIAS hello_library)
```

## 05_build_type
* CMake具有许多内置的构建配置，可用于编译工程。 这些配置指定了代码优化的级别，以及调试信息是否包含在二进制文件中，这些优化级别，主要有：
  * Release —— 不可以打断点调试，程序开发完成后发行使用的版本，占的体积小。 它对代码做了优化，因此速度会非常快，在编译器中使用命令： -O3 -DNDEBUG 可选择此版本
  * Debug ——调试的版本，体积大。在编译器中使用命令： -g 可选择此版本
  * MinSizeRel—— 最小体积版本。在编译器中使用命令：-Os -DNDEBUG可选择此版本
  * RelWithDebInfo—— 既优化又能调试。 在编译器中使用命令：-O2 -g -DNDEBUG可选择此版本

* 在命令行运行CMake的时候， 使用cmake命令行的-D选项配置编译类型：
```linux
cmake .. -DCMAKE_BUILD_TYPE=Release
```
* set 命令可以设置一些变量
  * 正常变量：
  ```linux
  set(<variable> <value>... [PARENT_SCOPE])
  ```
  设置的变量值 作用域属于整个 CMakeLists.txt 文件。(一个工程可能有多个CMakeLists.txt)

  当这个语句中加入PARENT_SCOPE后，表示要设置的变量是父目录中的CMakeLists.txt设置的变量。

  * cache 变量
    * 首先什么是CACHE变量，就是在运行cmake的时候，变量的值可能会被缓存到一份文件里即build命令下的CMakeCache.txt，当你重新运行cmake的时候，那些变量会默认使用这个缓存里的值。这个变量是全局变量，整个CMake工程都可以使用该变量。
    * 在这个文件里，只要运行cmake ..命令，自动会出现一些值，比如 CMAKE_INSTALL_PREFIX ，如果设置 ```set(CMAKE_INSTALL_PREFIX "/usr")``` ，虽然CACHE缓存文件里还有这个CMAKE_INSTALL_PREFIX 变量，但是因为我们显示得设置了一个名为CMAKE_INSTALL_PREFIX 的正常变量，所以之后使用CMAKE_INSTALL_PREFIX ，值是我们设置的正常变量的值。
    * 如果加上CACHE关键字，则设置的这个变量会被写入缓存文件中（但如果本身缓存文件中有这个变量，则不会覆盖缓存中的变量）。只有加上FORCE关键字，这个被写入文件的值会覆盖之前文件中存在的同名变量
    * 加上CACHE关键字，和是必需的
  ```linux
  set(<variable> <value>... CACHE <type> <docstring> [FORCE])
  
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)
  这句话，就是强制在缓存文件中覆盖CMAKE_BUILD_TYPE这个变量，将这个变量设置为RelWithDebInfo。而STRING "Choose the type of build."参数在使用cmake-gui的时候起作用，在界面上会出现一个下拉框供给用户选择来设置CMAKE_BUILD_TYPE变量。里的一行文字作为提示。
  ```

  * 环境变量
  ```linux
  set(ENV{<variable>} [<value>])
  ````
    * 设置一个 Environment Variable) 到给定值。随后的调用$ENV{<varible>}将返回此新值。
    * 此命令仅影响当前的CMake进程，不影响调用CMake的进程，也不影响整个系统环境，也不影响后续构建或测试过程的环境。
    * 如果在空字符串之后ENV{}或如果没有参数``，则此命令将清除环境变量的任何现有值。

## 06_compile_type
* 首先说一下什么是编译标志（或者 叫编译选项）。可执行文件的生成离不开编译和链接，那么如何编译，比如编译时使用C++的哪一个标准？这些编译设置都在CMAKE_CXX_FLAGS变量中。（C语言编译选项是CMAKE_C_FLAGS）

### 设置每个目标编译标志
* 在现代CMake中设置C ++标志的推荐方法是专门针对某个目标（target）设置标志，可以通过target_compile_definitions（）函数设置某个目标的编译标志。
```linux
target_compile_definitions(cmake_examples_compile_flags
    PRIVATE EX3
)
```
* 如果这个目标是一个库（cmake_examples_compile_flags），编译器在编译目标时添加定义-DEX3 ，并且选择了范围PUBLIC或INTERFACE，该定义-DEX3也将包含在链接此目标（cmake_examples_compile_flags）的所有可执行文件中。 注意，本语句使用了PRIVATE，所以编译选项不会传递。

* 对于编译器选项，还可以使用target_compile_options（）函数
```linux
target_compile_options(<target> [BEFORE]
  <INTERFACE|PUBLIC|PRIVATE> [items1...]
  [<INTERFACE|PUBLIC|PRIVATE> [items2...] ...])
```

* 是给 target 添加编译选项， target 指的是由 add_executable()产生的可执行文件或 add_library()添加进来的库。<INTERFACE|PUBLIC|PRIVATE>指的是`[items...]` 选项可以传播的范围， PUBLIC and INTERFACE会传播 <target>的 INTERFACE_COMPILE_DEFINITIONS 属性， PRIVATE and PUBLIC 会传播 target 的 COMPILE_DEFINITIONS 属性。

### 设置默认编译标志
* 默认的CMAKE_CXX_FLAGS为空或包含适用于构建类型的标志。 要设置其他默认编译标志，如下使用
```linux
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DEX2" CACHE STRING "Set C++ Compiler Flags" FORCE)
```

* 强制设置默认C++编译标志变量为缓存变量，如CMake（五） build type所说，该缓存变量被定义在文件中，相当于全局变量，源文件中也可以使用这个变量。这个变量原本包含的参数仍然存在，只是添加了EX2。CACHE STRING `"Set C++ Compiler Flags"` FORCE命令是为了强制将CMAKE_CXX_FLAGS变量 放到CMakeCache.txt文件中。`"${CMAKE_CXX_FLAGS} -DEX2"`这个字符串可以保留原有的CMAKE_CXX_FLAGS中的参数，额外添加了一个EX2参数。注意写法：空格，并且参数前加了-D。类似设置CMAKE_CXX_FLAGS，还可以设置其他选项：
  * 设置C编译标志： CMAKE_C_FLAGS
  * 设置链接标志：CMAKE_LINKER_FLAGS.

### 设置CMake标志
* 在cmake命令行中：`cmake .. -DCMAKE_CXX_FLAGS="-DEX3"`


## 07_include_third_part
* 几乎所有不平凡的项目都将要求包含第三方库，头文件或程序。 CMake支持使用find_package（）函数查找这些工具的路径。 这将从CMAKE_MODULE_PATH中的文件夹列表中搜索格式为`FindXXX.cmake`的CMake模块。 在linux上，默认搜索路径将是`/usr/share/cmake/Modules`。

### Finding a Package
* 如上所述，find_package（）函数将从CMAKE_MODULE_PATH中的文件夹列表中搜索`FindXXX.cmake`中的CMake模块。 find_package参数的确切格式取决于要查找的模块。 这通常记录在FindXXX.cmake文件的顶部。

```linux
find_package(Boost 1.46.1 REQUIRED COMPONENTS filesystem system)
```

* 参数：
  * Boost-库名称。 这是用于查找模块文件FindBoost.cmake的一部分
  * 1.46.1 - 需要的boost库最低版本
  * REQUIRED - 告诉模块这是必需的，如果找不到会报错
  * COMPONENTS - 要查找的库列表。从后面的参数代表的库里找boost
  * 可以使用更多参数，也可以使用其他变量。 在后面的示例中提供了更复杂的设置。

### Checking if the package is found
* 大多数被包含的包将设置变量XXX_FOUND，该变量可用于检查软件包在系统上是否可用。
* 在此示例中，变量为Boost_FOUND：
```linux
if(Boost_FOUND)
    message ("boost found")
    include_directories(${Boost_INCLUDE_DIRS})
else()
    message (FATAL_ERROR "Cannot find Boost")
endif()
```

### Exported Variables
* 找到包后，它会自动导出变量，这些变量可以通知用户在哪里可以找到库，头文件或可执行文件。 与XXX_FOUND变量类似，它们与包绑定在一起，通常记录在FindXXX.cmake文件的顶部。

* 本例中的变量
```linux
Boost_INCLUDE_DIRS - boost头文件的路径
```

### Alias / Imported targets别名/导入目标
* 大多数modern CMake库在其模块文件中导出别名目标。 导入目标的好处是它们也可以填充包含目录和链接的库。 例如，从CMake v3.5开始，Boost模块支持此功能。 与使用自己的别名目标相似，模块中的别名可以使引用找到的目标变得更加容易。 对于Boost，所有目标均使用Boost ::标识符，然后使用子系统名称导出。 例如:
  * Boost::boost for header only libraries
  * Boost::system for the boost system library.
  * Boost::filesystem for filesystem library.
* 注意：与您自己的目标一样，这些目标包括它们的依赖关系，因此与Boost :: filesystem链接将自动添加Boost :: boost和Boost :: system依赖关系。
* 要链接到导入的目标，可以使用以下命令
```linux
target_link_libraries( third_party_include
      PRIVATE
          Boost::filesystem
  )
```

### Non-alias targets
* 尽管大多数现代库都使用导入的目标，但并非所有模块都已更新。 如果未更新库，则通常会发现以下可用变量：
  * xxx_INCLUDE_DIRS - 指向库的包含目录的变量。
  * xxx_LIBRARY - 指向库路径的变量。

* 然后可以将它们添加到您的target_include_directories和target_link_libraries中，如下所示：
```linux
# Include the boost headers
  target_include_directories(third_party_include
      PRIVATE ${Boost_INCLUDE_DIRS}
  )

  # link against the boost libraries
  target_link_libraries(third_party_include
      PRIVATE
      ${Boost_SYSTEM_LIBRARY}
      ${Boost_FILESYSTEM_LIBRARY}
  )
```


