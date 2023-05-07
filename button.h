#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include <string>
#include <SFML/Graphics.hpp>

class button_t
{
public:
    sf::Text text;
    sf::RectangleShape graphic;
    sf::Rect<int> hitbox;

    button_t(std::string buttonText, sf::Vector2f position);
    void setPosition(sf::Vector2f position);
    bool isPressed(const sf::RenderWindow &window);
};

#endif // BUTTON_H_INCLUDED
