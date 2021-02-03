# add_subdirectory(myfuns)

add_library(myfuns1 myfuns1.cpp)
target_include_directories(myfuns1
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
          )
install(TARGETS myfuns1 DESTINATION lib)

add_library(myfuns2 myfuns2.cpp)
target_include_directories(myfuns2
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
          )
install(TARGETS myfuns2 DESTINATION lib)