# add_subdirectory(myfuns)

add_library(myfuns1 myfuns1.cpp)
install(TARGETS myfuns1 DESTINATION lib)
install(FILES myfuns1.h DESTINATION include)

add_library(myfuns2 myfuns2.cpp)
install(TARGETS myfuns2 DESTINATION lib)
install(FILES myfuns2.h DESTINATION include)