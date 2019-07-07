cmake_minimum_required(VERSION 3.0)

project(Image2Tandy)

set(CMAKE_CXX_STANDARD 17)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Wextra -Werror -O3")
endif()

add_executable(${PROJECT_NAME}
    main.cpp
    animation_loader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/lodepng/lodepng.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)

