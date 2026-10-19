#ifndef SHAPES_H
#define SHAPES_H

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <vector>
#include <utility>
#include <cmath>

class Shape{
    public:
        Shape(const ImVec2 start, const ImVec2 end, std::vector<float> color): startPos(start), endPos(end), borderColors(color){};
        ImVec2 getStartPos() {return startPos;}
        ImVec2 getEndPos() {return endPos;}
        std::vector<float> getBorderColors() {return borderColors;}
        virtual void draw(ImDrawList* dl) const = 0;
        virtual void draw(sf::RenderTarget& target) const = 0;
        virtual ~Shape() = default;
    protected:

        ImU32 borderU32() const {
            return IM_COL32((int)(borderColors[0] * 255.0f + 0.5f),
                        (int)(borderColors[1] * 255.0f + 0.5f),
                        (int)(borderColors[2] * 255.0f + 0.5f), 255);
        }

        sf::Color borderSfColor() const {
            return sf::Color(
            static_cast<std::uint8_t>(borderColors[0] * 255.f + 0.5f),
            static_cast<std::uint8_t>(borderColors[1] * 255.f + 0.5f),
            static_cast<std::uint8_t>(borderColors[2] * 255.f + 0.5f)
            );
        }
        ImVec2 startPos;
        ImVec2 endPos;
        std::vector<float> borderColors;

        
};

class Rect: public Shape{
    public:
        Rect(ImVec2 s, ImVec2 e, std::vector<float> border,
        std::vector<float> fill = {}, bool filled = false): Shape(s, e, std::move(border)), fillColors(std::move(fill)), filled(filled) {}

        void draw(ImDrawList* dl) const override{
            ImVec2 a(std::min(startPos.x, endPos.x), std::min(startPos.y, endPos.y));
            ImVec2 b(std::max(startPos.x, endPos.x), std::max(startPos.y, endPos.y));



            if (filled) {
                ImU32 f = IM_COL32((int)(fillColors[0] * 255.0f + 0.5f),
                               (int)(fillColors[1] * 255.0f + 0.5f),
                               (int)(fillColors[2] * 255.0f + 0.5f), 255);
                dl->AddRectFilled(a, b, f);
            }
        dl->AddRect(a, b, borderU32(), 0.0f, 0, 2.0f);
        }

        void draw(sf::RenderTarget& target) const override{
            float left = std::min(startPos.x, endPos.x);
            float top = std::min(startPos.y, endPos.y);
            float width = std::abs(endPos.x - startPos.x);
            float height = std::abs(endPos.y - startPos.y);

            sf::RectangleShape rect({width, height});
            rect.setPosition({left, top});
            rect.setOutlineColor(borderSfColor());
            rect.setOutlineThickness(2.f);

            if (filled) {
                rect.setFillColor(sf::Color(
                    static_cast<std::uint8_t>(fillColors[0] * 255.f + 0.5f),
                    static_cast<std::uint8_t>(fillColors[1] * 255.f + 0.5f),
                    static_cast<std::uint8_t>(fillColors[2] * 255.f + 0.5f)
                ));
            } else {
                rect.setFillColor(sf::Color::Transparent);
            }

            target.draw(rect);
        }
        
    private:
        std::vector<float> fillColors;
        bool filled;

};

class Line: public Shape{
    public:
        Line(const ImVec2 start, const ImVec2 end, const std::vector<float> color): Shape(start, end, color) {}
        void draw(ImDrawList* dl) const override{
                dl -> AddLine(startPos, endPos, borderU32(), 2.0f);
        }

        void draw(sf::RenderTarget& target) const override{
             sf::Vertex line[] = {
                {
                    sf::Vector2f{startPos.x, startPos.y},
                    borderSfColor()
                },
                {
                sf::Vector2f{endPos.x, endPos.y},
                borderSfColor()
                }
            };

             target.draw(line, 2, sf::PrimitiveType::Lines);
        }

};

class Circle: public Shape{
    public:
        Circle(const ImVec2 start, const ImVec2 end, const std::vector<float> color): Shape(start, end, color){
            center.x = (start.x + end.x)/2;
            center.y = (start.y + end.y)/2;
        }
        
        void draw(ImDrawList* dl) const override{
                float dx = endPos.x - center.x;
                float dy = endPos.y - center.y;
                float radius = std::sqrt(dx * dx + dy * dy);

                dl->AddCircle(center, radius, borderU32(), 0, 2.0f);
        }
        void draw(sf::RenderTarget& target) const override{
            float dx = endPos.x - center.x;
            float dy = endPos.y - center.y;
            float radius = std::sqrt(dx * dx + dy * dy);

            sf::CircleShape circle(radius);
            circle.setOrigin({radius, radius});
            circle.setPosition({center.x, center.y});
            circle.setFillColor(sf::Color::Transparent);
            circle.setOutlineColor(borderSfColor());
            circle.setOutlineThickness(2.f);

            target.draw(circle);
        }
    private:
        ImVec2 center;
};


#endif