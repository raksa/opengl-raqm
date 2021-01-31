cmake_minimum_required(VERSION 3.11)
project(test VERSION 0.1.0)

find_package(PkgConfig REQUIRED) 
pkg_check_modules(MY_RAQM REQUIRED IMPORTED_TARGET raqm)

set(CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(main ${CURRENT_SOURCE_DIR}/main.cpp)

target_sources(main PUBLIC 
    ${CURRENT_SOURCE_DIR}/funs.cpp
    ${CURRENT_SOURCE_DIR}/utils.cpp
    ${CURRENT_SOURCE_DIR}/render_text.cpp
    ${CURRENT_SOURCE_DIR}/load_shader.cpp
    ${CURRENT_SOURCE_DIR}/raqm_utils.c
)

target_link_libraries(main PUBLIC
        PkgConfig::MY_RAQM)

set(CMAKE_CXX_FLAGS "-w -framework glut -framework opengl")