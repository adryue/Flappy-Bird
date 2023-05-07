#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "commonDef.h"
#include "button.h"

using namespace std;

button_t::button_t(string buttonText, sf::Vector2f position)
{
    text.setFont(font);
    text.setFillColor(sf::Color::White);
    text.setOutlineThickness(5);
    text.setOutlineColor(sf::Color::Black);
    text.setString(buttonText);

    graphic.setSize(sf::Vector2f(text.getLocalBounds().width + text.getLocalBounds().left, text.getLocalBounds().height + text.getLocalBounds().top));
    graphic.setFillColor(sf::Color::Red);

    hitbox.width = text.getLocalBounds().width;
    hitbox.height = text.getLocalBounds().height;

    setPosition(position);
}
void button_t::setPosition(sf::Vector2f position)
{
    text.setPosition(position.x - (text.getLocalBounds().left + text.getLocalBounds().width) / 2,
                     position.y - (text.getLocalBounds().top + text.getLocalBounds().height) / 2);
    graphic.setPosition(position.x - (graphic.getLocalBounds().left + graphic.getLocalBounds().width) / 2,
                        position.y - (graphic.getLocalBounds().top + graphic.getLocalBounds().height) / 2);
    hitbox.left = position.x - (hitbox.width / 2);
    hitbox.top = position.y - (hitbox.height / 2);
}
bool button_t::isPressed(const sf::RenderWindow &window)
{
    return hitbox.contains(sf::Mouse::getPosition(window));
}
