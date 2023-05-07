#include <iostream>
#include <SFML/Graphics.hpp>
#include "button.h"
#include "home.h"

using namespace std;

screenName_t homeScreen(sf::RenderWindow &window)
{
    //title text
    sf::Text title("FLAPPY BIRD", font, 100);
    title.setFillColor(sf::Color::White);
    title.setOrigin(
        (title.getLocalBounds().width  / 2) + title.getLocalBounds().left,
        (title.getLocalBounds().height / 2) + title.getLocalBounds().top
    ); //set the origin to the middle of the text
    title.setPosition(WIN_X_LEN / 2, WIN_Y_LEN / 3);

    /*sf::Text letters[96];
    for (int i = 0; i < 96; i++)
    {
        letters[i].setFont(font);
        letters[i].setFillColor(sf::Color::White);
        letters[i].setString(string(1, (char)(i + ' ')));
        letters[i].setCharacterSize(100);
    }*/

    button_t playButton("Play", sf::Vector2f(WIN_X_LEN / 2, WIN_Y_LEN / 3 * 2));

    button_t settingsButton("Settings", sf::Vector2f(WIN_X_LEN / 2, WIN_Y_LEN * 0.75));

    while (window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return screenName_t::none;
            }
            //check if the player has clicked any buttons
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                if (playButton.isPressed(window))
                {
                    return screenName_t::play;
                }
                if (settingsButton.isPressed(window))
                {
                    return screenName_t::settings;
                }
            }
        }
        window.clear();

        //draw everything
        window.draw(playButton.graphic);
        window.draw(playButton.text);
        window.draw(settingsButton.graphic);
        window.draw(settingsButton.text);
        window.draw(title);

        /*for (int i = 0; i < 96; i++)
        {
            window.draw(letters[i]);
        }*/
        window.display();
    }
    return screenName_t::none;
}
