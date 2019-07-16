# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

set(distribution_DIR ${CMAKE_SOURCE_DIR}/../../../../src/main/jniLibs)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
#判断编译器类型,如果是gcc编译器,则在编译选项中加入c++11支持
if(CMAKE_COMPILER_IS_GNUCXX)

    set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
    message(STATUS "optional:-std=c++11")
endif(CMAKE_COMPILER_IS_GNUCXX)


# 配置路径 so 的头文件（h）的路径引用
include_directories(src/main/jniLibs/include)
# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

# 加载第三方的so库文件
add_library(
        opencvc7 #最终在build中生成的so名字
        SHARED
        IMPORTED)
set_target_properties(
        opencvc7 #最终在build中生成的so名字
        PROPERTIES IMPORTED_LOCATION
        ../../../../src/main/jniLibs/x86_64/libopencv_java4.so) #so的路径

# 代表最终会在build中生成一个so文件名为native-lib//所以如果我们自己添加的第三方so包，要重新 add_library
add_library( # Sets the name of the library.
        native-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        src/main/cpp/native-lib.cpp src/main/cpp/BitmapUtils.cpp)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        native-lib
        opencvc7
        # 加上下面这一行
        jnigraphics
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})