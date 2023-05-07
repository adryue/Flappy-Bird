#ifndef TEXTBOX_H_INCLUDED
#define TEXTBOX_H_INCLUDED

#include <string>
#include <SFML/Graphics.hpp>
#include "commonDef.h"

class textBox_t : public sf::Text
{
public:
    textBox_t(std::string text, unsigned int textSize, sf::Vector2f position);
};

#endif // TEXTBOX_H_INCLUDED
