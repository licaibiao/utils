cmake_minimum_required(VERSION 3.5)


set(MAIN_SRC main.c)

ADD_EXECUTABLE(main  ${MAIN_SRC})

target_link_libraries(main hello1 hello2)

