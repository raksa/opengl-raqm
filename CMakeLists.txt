cmake_minimum_required(VERSION 3.11)
project(test VERSION 0.1.0)

add_executable(main ${CMAKE_CURRENT_SOURCE_DIR}/src/main_opengl.cpp)
target_sources(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/utils.c)

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

# silence warning messages
set(CMAKE_CXX_FLAGS "-w -framework glut -framework opengl")