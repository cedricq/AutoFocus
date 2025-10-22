cmake_minimum_required(VERSION 3.14)
project(autofocus_run)

add_executable (${PROJECT_NAME}_test autofocus_integration_t.cpp )
target_link_libraries(${PROJECT_NAME}_test gtest_main autofocus)
add_test(${PROJECT_NAME}_integ ${PROJECT_NAME}_test --gtest_color=yes)

add_executable (${PROJECT_NAME} autofocus_app.cpp )
target_link_libraries(${PROJECT_NAME} autofocus)



