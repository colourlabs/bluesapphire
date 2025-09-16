cmake_minimum_required(VERSION 3.16)
project(BlueSapphire LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.15.3
)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        3.4
)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(spdlog)
FetchContent_MakeAvailable(glfw)

add_library(BlueSapphire STATIC
    src/App.cpp
    src/utils/Logger.cpp
)

target_include_directories(BlueSapphire
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

find_package(OpenGL REQUIRED)

option(BS_BUILD_TESTS "build example games" ON)

if(BS_BUILD_TESTS)
    add_subdirectory(examples)
endif()

target_link_libraries(BlueSapphire
    PUBLIC
        spdlog::spdlog
        glfw
        OpenGL::GL
)

target_compile_features(BlueSapphire PUBLIC cxx_std_20)