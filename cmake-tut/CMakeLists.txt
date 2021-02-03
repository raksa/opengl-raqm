# cmake . && cmake --build .
cmake_minimum_required(VERSION 3.0)
project(Test VERSION 0.0.1)
add_executable(main main.cpp)

add_subdirectory(${PROJECT_SOURCE_DIR}/myfuns)

# cmake . && cmake --build . && cmake --install . --prefix "./dist"
install(TARGETS main DESTINATION bin)

target_include_directories(main PRIVATE ${PROJECT_SOURCE_DIR}/myfuns)
target_link_libraries(main PRIVATE myfuns1)
target_link_libraries(main PRIVATE myfuns2)

# cmake . && cmake --build . && cmake --install . --prefix "./dist" && ctest -C Debug -VV
enable_testing()
add_test(NAME Runs COMMAND main)