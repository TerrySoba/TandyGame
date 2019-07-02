cmake_minimum_required(VERSION 3.0)

project(Image2Tandy)

add_executable(${PROJECT_NAME}
    main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/lodepng/lodepng.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)

