cmake_minimum_required(VERSION 3.15)

project(proceduralX LANGUAGES CXX VERSION 1.0)

find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_mixer REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB SOURCES_GAME "src/*.cpp")

add_executable(proceduralX ${SOURCES_GAME})

include_directories("/usr/local/include/noise")

target_link_libraries(proceduralX PRIVATE /usr/local/lib/libnoise-static.a)

target_link_libraries(proceduralX PRIVATE SDL3::SDL3)
target_link_libraries(proceduralX PRIVATE SDL3_ttf::SDL3_ttf)
target_link_libraries(proceduralX PRIVATE SDL3_image::SDL3_image)
target_link_libraries(proceduralX PRIVATE SDL3_mixer::SDL3_mixer)

target_compile_definitions(proceduralX PRIVATE $<$<CONFIG:Debug>:DEBUG>)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Building in Debug mode")
else()
    message(STATUS "Building in Release mode")
endif()

add_compile_options(-Wall -Wextra -pedantic)
