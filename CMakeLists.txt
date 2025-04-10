cmake_minimum_required(VERSION 3.30)

project(highlands)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(X11 REQUIRED)

add_executable(highlands src/main.cpp src/glad.c imgui/imgui.cpp imgui/imgui_demo.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp)

target_include_directories(highlands PRIVATE include/ ${X11_INCLUDE_DIR})
target_link_libraries(highlands PRIVATE glfw OpenGL::GL X11::X11)

target_compile_definitions(
  highlands PRIVATE SHADER_VERT="resources/shaders/shader.vert"
                    SHADER_FRAG="resources/shaders/shader.frag")
