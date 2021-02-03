# https://cmake.org/cmake/help/latest/guide/tutorial/index.html#build-and-test
# cmake . && cmake --build .
cmake_minimum_required(VERSION 3.10)

project(cmake_tut VERSION 0.1)
configure_file(main.h.in main.h)

set(CMAKE_BUILD_TYPE Debug)

add_executable(main main.cpp)

add_subdirectory(${PROJECT_SOURCE_DIR}/gen-table)
add_subdirectory(${PROJECT_SOURCE_DIR}/myfuns)
add_subdirectory(${PROJECT_SOURCE_DIR}/mylib)

# add compile definitions
target_compile_definitions(mylib PRIVATE "SHOULD_NOT_SAY")
# global defined
add_definitions(-DMAIN_SHOULD_NOT_SAY)

# cmake . && cmake --build . && cmake --install . --prefix "./dist"
install(TARGETS main DESTINATION bin)

target_link_libraries(main PRIVATE myfuns1)
target_link_libraries(main PRIVATE myfuns2)
target_link_libraries(main PRIVATE mylib)

target_include_directories(main PRIVATE ${PROJECT_SOURCE_DIR}/mylib)

# cmake . && cmake --build . && cmake --install . --prefix "./dist" && ctest -C Debug -VV
enable_testing()
function(do_test target arg result)
    add_test(NAME Runs-with-${arg} COMMAND ${target} ${arg})
    set_tests_properties(Runs-with-${arg}
      PROPERTIES PASS_REGULAR_EXPRESSION ${result}
      )
endfunction(do_test)

do_test(main "" "main add|mult")
do_test(main add "add\\(2, 3\\) = 5")
do_test(main mult "mult\\(2, 3\\) = 6")
do_test(main sub "Invalid operation")


# cmake . && cpack -G ZIP -C Debug
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${Tutorial_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${Tutorial_VERSION_MINOR}")
include(CPack)