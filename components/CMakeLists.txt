cmake_minimum_required(VERSION 3.14)
project(autofocus)

add_library(${PROJECT_NAME} 
    src/CalibrationData.cpp 
    src/DepthSensor.cpp
    src/Camera.cpp
    )
target_include_directories(${PROJECT_NAME} 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
        ${OpenCV_INCLUDE_DIRS}
    )
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS})

add_subdirectory(test)
