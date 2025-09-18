cmake_minimum_required(VERSION 3.16)
project(BlueSapphire LANGUAGES CXX C)

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

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e
)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glm)
FetchContent_MakeAvailable(spdlog)
FetchContent_MakeAvailable(glfw)

add_library(glad STATIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/vendor)
set_target_properties(glad PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
)

file(GLOB_RECURSE BS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# copy shaders and assets to build directory
file(GLOB SHADERS "src/shaders/*")
file(COPY ${SHADERS} DESTINATION ${CMAKE_BINARY_DIR}/shaders)

file(GLOB SHADERS "src/assets/*")
file(COPY ${SHADERS} DESTINATION ${CMAKE_BINARY_DIR}/assets)


add_library(BlueSapphire STATIC
    ${BS_SOURCES}
)

target_include_directories(BlueSapphire
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/vendor>
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
        glad
        OpenGL::GL
        glm::glm
)

target_compile_features(BlueSapphire PUBLIC cxx_std_20)