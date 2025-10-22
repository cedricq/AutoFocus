cmake_minimum_required(VERSION 3.14)
project(unit_tests)

add_executable (${PROJECT_NAME} CalibrationData_t.cpp AutoFocus_t.cpp)
target_link_libraries(${PROJECT_NAME} gtest_main autofocus)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/data
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/data
)

add_test(${PROJECT_NAME} ${PROJECT_NAME} --gtest_color=yes)

