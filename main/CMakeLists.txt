cmake_minimum_required(VERSION 3.14)
project(autofocus_main)

add_executable (${PROJECT_NAME}
    main_app.cpp 
    )
target_link_libraries(${PROJECT_NAME} autofocus)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/data
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/data
)

