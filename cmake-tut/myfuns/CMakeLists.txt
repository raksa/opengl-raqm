# add_subdirectory(myfuns)

add_library(myfuns1 myfuns1.cpp)
install(TARGETS myfuns1 DESTINATION lib)

add_library(myfuns2 myfuns2.cpp)
install(TARGETS myfuns2 DESTINATION lib)