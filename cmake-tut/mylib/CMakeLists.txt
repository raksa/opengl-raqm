# add_subdirectory(myfuns)

add_library(mylib mylib.cpp)
install(TARGETS mylib DESTINATION lib)
install(FILES mylib.h DESTINATION include)