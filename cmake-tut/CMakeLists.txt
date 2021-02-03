# cmake . && cmake --build .
cmake_minimum_required(VERSION 3.0)
project(Test VERSION 0.0.1)
add_executable(main main.cpp)

add_subdirectory(myfuns)

# cmake . && cmake --build . && cmake --install . --prefix "./dist"
install(TARGETS main DESTINATION bin)

# cmake . && cmake --build . && cmake --install . --prefix "./dist" && ctest -C Debug -VV
enable_testing()
add_test(NAME Runs COMMAND man)