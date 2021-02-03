# add_subdirectory(myfuns)
add_library(myfuns1 myfuns1.cpp)
install(TARGETS myfuns1 DESTINATION lib)