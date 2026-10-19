
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include "ImGuiFileDialog.h"
#include <vector>
#include <optional>
#include <utility>
#include "Shapes.h"
#include <iostream>
#include <fstream>

enum class Tool{
    Line,
    Rectangle,
    Filled_Rectangle,
    Circle,
    None
};

int main()
{
    sf::RenderWindow window(
        sf::VideoMode({1800u, 900u}),
        ""
    );

    window.setFramerateLimit(60);

    if (!ImGui::SFML::Init(window))
        return 1;

    sf::Clock deltaClock;
    bool showWindow = true;
    bool showTools = false;

    std::string fileName;

    //Textures
    sf::Texture brush("img/Brush.png");
    sf::Sprite brush_sprite(brush);

    sf::Texture rectangle("img/Rectangle.png");
    sf::Sprite rectangle_sprite(rectangle);

    sf::Texture rectangle_filled("img/Rectangle_filled.png");
    sf::Sprite rectangle_filled_sprite(rectangle_filled);

    sf::Texture circle("img/Circle.png");
    sf::Sprite Circle_sprite(circle);

    //Color Pickers
    static float border_color[3] = {0.2f, 0.6f, 1.0f};
    static float fill_color[3] = {0.2f, 0.6f, 1.0f};

    //Drawing SetUp
    static bool drawing = false;
    static ImVec2 lineStart;
    static ImVec2 lineEnd;
    Tool currentTool = Tool::None;

    std::vector<std::unique_ptr<Shape>> shapes;


    bool saveRequested = false;
    bool hideUI = saveRequested;
    bool clearToSave = false;

    //FIles
    sf::Texture loadedTex;
    std::optional<sf::Sprite> loadedSprite(loadedTex);
    std::string new_file;
    sf::Texture tex(window.getSize());

    bool showInfo = false;

    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>())
                window.close();
        }

        ImGui::SFML::Update(window, deltaClock.restart());


        if(!hideUI && ImGui::BeginMainMenuBar())
        {
            if(ImGui::BeginMenu("File")){
                if(ImGui::MenuItem("New")){
                    shapes.clear();
                    loadedSprite.reset();
                    drawing = false;
                }
                if(ImGui::MenuItem("Open")){
                    IGFD::FileDialogConfig config;
                    config.path = ".";
                    ImGuiFileDialog::Instance()->OpenDialog("OpenFileDialog", "Open", ".png", config);

                }
                if(ImGui::MenuItem("Save As"))  {
                    saveRequested = true;
                }
                if(ImGui::MenuItem("Exit"))  window.close();

                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("View")){
                ImGui::MenuItem("Show Tool Option", nullptr, &showTools);
                ImGui::EndMenu();
            }

            if(ImGui::BeginMenu("Help")){
                ImGui::MenuItem("About", nullptr, &showInfo);
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        ImVec2 mousePos = ImGui::GetMousePos();
        bool overMenu = mousePos.y < ImGui::GetFrameHeight();

        bool shapeTool = currentTool == Tool::Line
              || currentTool == Tool::Rectangle
              || currentTool == Tool::Filled_Rectangle
              || currentTool == Tool::Circle;

            if (shapeTool && !overMenu && !ImGui::GetIO().WantCaptureMouse)
            {
                if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                {   
                    drawing = true;
                    lineStart = lineEnd = mousePos;
                }
            }

        if (drawing && ImGui::IsMouseDown(ImGuiMouseButton_Left))
        {
            lineEnd = mousePos;
        }

        if (drawing && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
        {
            lineEnd = mousePos;
                std::vector<float> border = {border_color[0], border_color[1], border_color[2]};
                std::vector<float> fill   = {fill_color[0],   fill_color[1],   fill_color[2]};

            switch (currentTool) {
                    case Tool::Line:
                        shapes.push_back(std::make_unique<Line>(lineStart, lineEnd, border));
                        break;
                    case Tool::Rectangle:
                        shapes.push_back(std::make_unique<Rect>(lineStart, lineEnd, border, fill, false));
                        break;
                    case Tool::Filled_Rectangle:
                        shapes.push_back(std::make_unique<Rect>(lineStart, lineEnd, border, fill, true));
                        break;
                    case Tool::Circle:
                        shapes.push_back(std::make_unique<Circle>(lineStart, lineEnd, border));
                        break;     
                    default: break;
            }
            drawing = false;
        }

        ImDrawList* drawList = ImGui::GetBackgroundDrawList();

        //Petla do rysowanie figur
        for (const auto& shape : shapes)
            shape->draw(drawList);

        
        if (drawing)
        {
            std::vector<float> border = {border_color[0], border_color[1], border_color[2]};
            std::vector<float> fill   = {fill_color[0],   fill_color[1],   fill_color[2]};

            if (currentTool == Tool::Line)
                Line(lineStart, lineEnd, border).draw(drawList);
            else if (currentTool == Tool::Rectangle)
                Rect(lineStart, lineEnd, border, fill, false).draw(drawList);
            else if (currentTool == Tool::Filled_Rectangle)
                Rect(lineStart, lineEnd, border, fill, true).draw(drawList);
            else if (currentTool == Tool::Circle)
                Circle(lineStart, lineEnd, border).draw(drawList);
        }

        if (showWindow)
        {
            if(!hideUI && showInfo){
                ImGui::Begin("About");
                ImGui::Text("Author: Jakub Wolczynski");
                if(ImGui::Button("Ok")) showInfo = false;
                ImGui::End();
            }

            if(!hideUI && showTools){

                ImGui::Begin("Tools", &showTools);
                if(ImGui::ImageButton("Brush", brush_sprite, {64, 64}, {0, 0, 0})){
                    currentTool = Tool::Line;
                }
                ImGui::SameLine();
                if(ImGui::ImageButton("Rectangle", rectangle_sprite, {64, 64}, {0, 0, 0})){
                     currentTool = Tool::Rectangle;
                }
                if(ImGui::ImageButton("Rectangle Filled", rectangle_filled_sprite, {64, 64}, {0, 0, 0})){
                    currentTool = Tool::Filled_Rectangle;
                }
                ImGui::SameLine();
                if(ImGui::ImageButton("Circle", Circle_sprite, {64, 64}, {0, 0, 0})){
                    currentTool = Tool::Circle;
                }

                ImGui::Text("Border COLOR:");
                ImGui::ColorEdit3("Border", border_color);
                ImGui::Text("Fill COLOR:");
                ImGui::ColorEdit3("Fill", fill_color);
                ImGui::End();
            }
            if (ImGuiFileDialog::Instance()->Display("OpenFileDialog", ImGuiWindowFlags_NoCollapse, ImVec2(700, 400))) 
            {
                    if (ImGuiFileDialog::Instance()->IsOk()) {
                        fileName = ImGuiFileDialog::Instance()->GetFilePathName();

                        sf::Image img;
                        if (img.loadFromFile(fileName)) {
                            const sf::Vector2u sz = img.getSize();
                            for (unsigned x = 0; x < sz.x; ++x)
                            for (unsigned y = 0; y < sz.y; ++y) {
                            sf::Color c = img.getPixel({x, y});
                            c.a = 255;
                            img.setPixel({x, y}, c);
                        }

                        if (loadedTex.loadFromImage(img)) {
                            loadedSprite.emplace(loadedTex);
                            loadedSprite->setPosition({0.f, 0.f});
                        } else {
                            std::cerr << "loadFromFile nie powiodlo sie\n";
                        }
                        }
                    }
                        ImGuiFileDialog::Instance()->Close();
            }
    }

        if (saveRequested && !hideUI)
        {
            IGFD::FileDialogConfig config;
            config.path = ".";
            ImGuiFileDialog::Instance()->OpenDialog("SaveFile","Save File",".png", config);

            showTools = false;
        }

        if(ImGuiFileDialog::Instance()->Display(
            "SaveFile", ImGuiWindowFlags_NoCollapse,ImVec2(700, 400)))
            {
            if (ImGuiFileDialog::Instance()->IsOk())
            {
                new_file = ImGuiFileDialog::Instance()->GetFilePathName();
            }
            else
            {
                saveRequested = false;
                hideUI = false;
                showTools = true;
            }   

            ImGuiFileDialog::Instance()->Close();
        }

        window.clear(sf::Color(30, 30, 35));
        if (loadedSprite)
            window.draw(*loadedSprite);
        
        ImGui::SFML::Render(window);

        //Zapis do Pliku
        if (saveRequested && !new_file.empty())
        {
            sf::RenderTexture canvas(window.getSize());
            canvas.clear(sf::Color(30, 30, 35));

            if(loadedSprite)
                canvas.draw(*loadedSprite);
            for(const auto& shape: shapes)
                shape -> draw(canvas);
            
            canvas.display();

            if(!canvas.getTexture().copyToImage().saveToFile(new_file))
                std::cerr << "Nie mozna zapisac pliku";

            saveRequested = false;
            hideUI = false;
            showTools = true;
            new_file.clear();
        }
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}
