cmake_minimum_required(VERSION 3.20)
project(Editor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_STATIC_LIBRARIES ON)
find_package(SFML 3.0 COMPONENTS Graphics Window System REQUIRED)

add_executable(Editor
    src/main.cpp
    src/Shapes.cpp

    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui-SFML.cpp
    imgui/ImGuiFileDialog.cpp
)

target_include_directories(Editor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/include

)

target_link_libraries(Editor PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)

file(COPY ${CMAKE_SOURCE_DIR}/img
     DESTINATION ${CMAKE_BINARY_DIR})