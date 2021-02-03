# add_subdirectory(myfuns)
add_library(myfuns myfuns.cpp)
install(TARGETS myfuns DESTINATION lib)