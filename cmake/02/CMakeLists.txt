#最低版本控制
cmake_minimum_required(VERSION 3.5)

#工程名字
project(HELLO)

set(LIB_SRC hello.c)
set(APP_SRC main.c)

#生成静态库
add_library(libhello ${LIB_SRC})

#生成可执行文件
add_executable(hello ${APP_SRC})

#执行文件链接库文件
target_link_libraries(hello libhello)

#重命名库文件的名字
set_target_properties(libhello PROPERTIES OUTPUT_NAME "hello")
