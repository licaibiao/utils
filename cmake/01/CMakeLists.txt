project(HELLO)

message(${PROJECT_SOURCE_DIR})

set(SRC_LIST main.c)
add_executable(hello ${SRC_LIST})
