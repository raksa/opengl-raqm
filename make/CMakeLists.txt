cmake_minimum_required(VERSION 3.0.0)
project(test VERSION 0.1.0)

add_executable(main src/main.cpp)
target_sources(main PUBLIC src/utils.c)

include_directories(/usr/local/opt/freetype/include/freetype2)
find_library(FREETYPE_LIBRARY
    freetype
)
target_link_libraries(main PUBLIC ${FREETYPE_LIBRARY})

include_directories(../../src)
find_library(RAQM_LIBRARY
    raqm
    PATH ../../src/.libs
)
target_link_libraries(main PUBLIC ${RAQM_LIBRARY})
