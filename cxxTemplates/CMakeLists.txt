cmake_minimum_required(VERSION 3.5)

project(testsrc)

set(CMAKE_CXX_STANDARD 14)

SET(CMAKE_BUILD_TYPE DEBUG)                #调试模式这句话必不可少！

add_executable(testsrc main.cxx)
target_link_libraries(testsrc pthread)    #多线程要链接库文件pthread
find_package(Boost REQUIRED)