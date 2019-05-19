
cmake_minimum_required(VERSION 3.4.1)

add_library(
        jpeg-yk
        SHARED
        src/main/cpp/jpeg-yk.cpp)

add_library(libjpeg STATIC IMPORTED)

set_target_properties(libjpeg PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/src/main/cpp/libs/armeabi-v7a/libturbojpeg.a)
#set_target_properties(libjpeg PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/src/main/cpp/libs/armeabi/libturbojpeg.a)

#引入头文件    import
include_directories(src/main/cpp/include)

target_link_libraries(
        jpeg-yk
        libjpeg
        #jnigraphics是安卓NDK目录中直接有的
        jnigraphics
        log)