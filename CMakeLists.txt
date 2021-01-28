cmake_minimum_required(VERSION 3.11)
project(test VERSION 0.1.0)

set(CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(main ${CURRENT_SOURCE_DIR}/main_opengl.cpp)

target_sources(main PUBLIC 
    ${CURRENT_SOURCE_DIR}/main_opengl_utils.cpp
    ${CURRENT_SOURCE_DIR}/render_text.cpp
    ${CURRENT_SOURCE_DIR}/utils.c
)

include_directories(/usr/local/opt/freetype/include/freetype2)
find_library(FREETYPE_LIBRARY
    freetype
)
target_link_libraries(main PUBLIC ${FREETYPE_LIBRARY})

include_directories(libs/raqm/headers)
find_library(RAQM_LIBRARY
    raqm
    PATH libs/raqm
)
target_link_libraries(main PUBLIC ${RAQM_LIBRARY})

set(CMAKE_CXX_FLAGS "-w -framework glut -framework opengl")