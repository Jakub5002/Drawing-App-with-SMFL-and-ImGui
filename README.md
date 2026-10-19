# Simple Graphic Editor
### with usage of SMFL and Dear ImGui

A simple drawing application developed in C++ using SFML and Dear ImGui. The application allows users to draw basic geometric shapes, customize their colors, and save their drawings as PNG images.

### Features
* **Drawing tools**: Lines, rectangles, filled rectangles, and circles.

* **Color customization**: Choose border and fill colors.

* **Image loading**: Open PNG images and use them as a background.

* **Save drawings**: Export your artwork to PNG files without the user interface.

* **Graphical user interface**: Menu bar and tool panels built with Dear ImGui.

* **File dialogs**: Open and save files using ImGuiFileDialog.

### Technologies Used

* C++

* SFML 3 — graphics and window management

* Dear ImGui — graphical user interface

* ImGui-SFML — integration between SFML and Dear ImGui

* ImGuiFileDialog — file selection dialogs

* CMake — project build system

### Getting started

Make sure you have the following installed:

* C++17 compatible compiler

* CMake 3.20 or newer

* SFML 3.0 (Graphics, Window, System)

* Dear ImGui, ImGui-SFML and ImGuiFileDialog (included in the repository)

Note: SFML 3.0 must be installed and discoverable by CMake.

## Build
```bash
cmake -S . -B build
cmake --build build
```
## Run
On Linux
```bash
./build/Editor
```
On Windows, run the generated Editor.exe executable from the build directory (or its configuration-specific subdirectory).

## Usage

- Select a drawing tool from the toolbar.

- Click and drag to draw a shape.

- Customize border and fill colors using the available color controls.

- Use the File menu to open a PNG image or save your drawing.

- Save your drawing as a PNG