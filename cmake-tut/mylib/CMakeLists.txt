# add_subdirectory(myfuns)

add_library(mylib mylib.cpp
)
add_dependencies(mylib gen-table)
target_include_directories(mylib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../gen-table)
install(TARGETS mylib DESTINATION lib)
install(FILES mylib.h DESTINATION include)