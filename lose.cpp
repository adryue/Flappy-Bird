#include <iostream>
#include <chrono>
#include <thread>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "button.h"
#include "textBox.h"
#include "lose.h"

using namespace std;

screenName_t loseScreen(sf::RenderWindow &window)
{
    //sounds
    sf::SoundBuffer loseSoundBuffer;
    loseSoundBuffer.loadFromFile("Sounds/Lose.wav");
    sf::Sound loseSound(loseSoundBuffer);
    loseSound.setVolume(10);
    loseSound.play();

    //text
    textBox_t loseText("GAME OVER", 50, sf::Vector2f(WIN_X_LEN / 2, WIN_Y_LEN / 4));

    textBox_t scoreText("SCORE: " + to_string(score), 30, sf::Vector2f(WIN_X_LEN / 2, WIN_Y_LEN / 4 + 50));

    highScore = max(score, highScore);
    textBox_t highScoreText("HIGHSCORE: " + to_string(highScore), 30, sf::Vector2f(WIN_X_LEN / 2, WIN_Y_LEN / 4 + 100));

    button_t replayButton("Play Again", sf::Vector2f(WIN_X_LEN / 2, WIN_Y_LEN / 3 * 2));
    button_t homeButton("Exit", sf::Vector2f(WIN_X_LEN / 2, WIN_Y_LEN / 3 * 2 + 50));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return screenName_t::none;
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                //check if the player has clicked any buttons
                if (replayButton.isPressed(window))
                {
                    return screenName_t::play;
                }
                if (homeButton.isPressed(window))
                {
                    return screenName_t::home;
                }
            }
        }
        window.clear();


        window.draw(loseText);
        window.draw(scoreText);
        window.draw(highScoreText);
        window.draw(replayButton.graphic);
        window.draw(replayButton.text);
        window.draw(homeButton.graphic);
        window.draw(homeButton.text);
        window.display();
    }
    return screenName_t::none;
}
