cmake_minimum_required(VERSION 3.14)
project(autofocus_test)

add_executable (${PROJECT_NAME} autofocus_t.cpp )
target_link_libraries(${PROJECT_NAME} gtest_main autofocus)
add_test(${PROJECT_NAME} ${PROJECT_NAME} --gtest_color=yes)


