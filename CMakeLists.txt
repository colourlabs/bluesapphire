cmake_minimum_required(VERSION 3.16)
project(BlueSapphire LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(BlueSapphire STATIC
    src/App.cpp
)

target_include_directories(BlueSapphire
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

option(BS_BUILD_TESTS "build example games" ON)

if(BS_BUILD_TESTS)
    add_subdirectory(examples)
endif()

target_compile_features(BlueSapphire PUBLIC cxx_std_20)