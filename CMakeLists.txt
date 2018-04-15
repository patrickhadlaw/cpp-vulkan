cmake_minimum_required(VERSION 3.0)
project(cpp-vulkan)

set(CPPVK_VERSION_MAJOR 0)
set(CPPVK_VERSION_MINOR 1)
set(CPPVK_VERSION_REVISION 1)

set(GLFW_LOCATION lib/glfw)

configure_file (
    "${PROJECT_SOURCE_DIR}/configuration.h.in"
    "${PROJECT_BINARY_DIR}/configuration.h"
)

file(GLOB_RECURSE sources src/*.cpp src/*.h)
file(GLOB_RECURSE resources resources/*)
file(GLOB_RECURSE shader shader/*)
file(GLOB_RECURSE lib lib/*)

add_executable(cpp-vulkan cpp-vulkan.cpp ${sources})
target_compile_options(cpp-vulkan PUBLIC -std=c++1y -Wall)

file(COPY ${resources} DESTINATION resources)
file(COPY ${shader} DESTINATION shader)

add_subdirectory(${GLFW_LOCATION})

find_package(Vulkan REQUIRED)
target_include_directories(cpp-vulkan PUBLIC src include ${GLFW_LOCATION}/include lib/glm ${VULKAN_INCLUDE_DIR})

#find_library(ASSIMP_LIBRARY assimp HINTS ${PROJECT_BINARY_DIR}/lib)

target_link_libraries(cpp-vulkan PUBLIC Vulkan::Vulkan glfw ${GLFW_LIBRARIES})

install(TARGETS cpp-vulkan DESTINATION bin)