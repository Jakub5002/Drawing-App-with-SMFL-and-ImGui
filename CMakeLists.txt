cmake_minimum_required(VERSION 3.20)
project(Lab02 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_STATIC_LIBRARIES ON)
find_package(SFML 3.0 COMPONENTS Graphics Window System REQUIRED)

add_executable(Lab02
    src/main.cpp
    src/Shapes.cpp

    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui-SFML.cpp
    imgui/ImGuiFileDialog.cpp
)

target_include_directories(Lab02 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/include

)

target_link_libraries(Lab02 PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)