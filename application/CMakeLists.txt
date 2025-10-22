cmake_minimum_required(VERSION 3.14)
project(autofocus)

add_library(${PROJECT_NAME} src/CalibrationData.cpp src/AutoFocus.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)

#target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)

add_subdirectory(test)
