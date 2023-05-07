#include <SFML/Graphics.hpp>
#include "slider.h"

const int KNOB_RADIUS = 10;
const sf::Vector2f PATH_SIZE(400, 20);

slider_t::slider_t(float minV, float maxV, float initV, float inc, sf::Vector2f position)
{
    minValue = minV;
    maxValue = maxV;
    increment = inc;

    isClicked = false;

    knob.setFillColor(sf::Color(100, 100, 100));
    knob.setRadius(KNOB_RADIUS);
    knob.setOrigin(KNOB_RADIUS, KNOB_RADIUS);
    knob.setPosition(position);

    path.setFillColor(sf::Color::White);
    path.setSize(PATH_SIZE);
    path.setPosition(position.x - (path.getLocalBounds().left + path.getLocalBounds().width) / 2,
                     position.y - (path.getLocalBounds().top + path.getLocalBounds().height) / 2);

    setValue(initV);
}

void slider_t::checkForClick(const sf::RenderWindow &window)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f knobPos = knob.getPosition();

    if ((mousePos.x - knobPos.x) * (mousePos.x - knobPos.x) + (mousePos.y - knobPos.y) * (mousePos.y - knobPos.y) <= KNOB_RADIUS * KNOB_RADIUS)
    {
        isClicked = true;
    }
}
void slider_t::followMouseIfClicked(const sf::RenderWindow &window)
{
    if (isClicked)
    {
        knob.setPosition(sf::Mouse::getPosition(window).x, knob.getPosition().y);
        ensureWithinBounds();
    }
}

float slider_t::getValue()
{
    return ((knob.getPosition().x - path.getPosition().x) / path.getLocalBounds().width) * (maxValue - minValue) + minValue;
}
void slider_t::setValue(float value)
{
    knob.setPosition(path.getPosition().x + (((value - minValue) / (maxValue - minValue)) * path.getLocalBounds().width), knob.getPosition().y);
    ensureWithinBounds();
}

void slider_t::ensureWithinBounds()
{
    if (knob.getPosition().x < path.getPosition().x)
    {
        knob.setPosition(path.getPosition().x, knob.getPosition().y);
    }
    else if (knob.getPosition().x > path.getPosition().x + path.getLocalBounds().width)
    {
        knob.setPosition(path.getPosition().x + path.getLocalBounds().width, knob.getPosition().y);
    }
}
