cmake_minimum_required(VERSION 3.30)

project(highlands)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(WIN32)
  set(PLATFORM "Windows")
elseif(APPLE)
  set(PLATFORM "macOS")
elseif(UNIX AND NOT APPLE)
  set(PLATFORM "Linux")
else()
  message(FATAL_ERROR "Unsupported platform!")
endif()

message(STATUS "Building for platform: ${PLATFORM}")

find_package(OpenGL REQUIRED)
add_executable(highlands src/main.cpp src/glad.c)
target_include_directories(highlands PRIVATE include/)

if(PLATFORM STREQUAL "Linux")
  find_package(glfw3 REQUIRED)
  find_package(X11 REQUIRED)
  target_include_directories(highlands PRIVATE ${X11_INCLUDE_DIR})
  target_link_libraries(highlands PRIVATE glfw OpenGL::GL X11::X11)

elseif(PLATFORM STREQUAL "Windows")
  find_package(glfw3 REQUIRED)
  target_compile_definitions(highlands PRIVATE _CRT_SECURE_NO_WARNINGS)
  target_link_libraries(highlands PRIVATE glfw OpenGL::GL opengl32)

elseif(PLATFORM STREQUAL "macOS")
  find_package(glfw3 REQUIRED)
  target_link_libraries(
    highlands PRIVATE glfw OpenGL::GL "-framework Cocoa" "-framework OpenGL"
                      "-framework IOKit" "-framework CoreVideo")
endif()
