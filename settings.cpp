#include <iostream>
#include <fstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "button.h"
#include "slider.h"
#include "textBox.h"
#include "settings.h"

using namespace std;

class birdTexture_t : public sf::Texture
{
public:
    string name;
};

void nextTexture(sf::Sprite &sprite, const birdTexture_t &texture)
{
    sprite.setTexture(texture);
    sprite.setScale(BIRD_LENGTH / texture.getSize().x, BIRD_WIDTH / texture.getSize().y);
    sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
}

screenName_t settingsScreen(sf::RenderWindow &window)
{
    ifstream fin;
    ofstream fout;

    //temporary variables
    float birdJumpSpeed = BIRD_JUMP_SPEED;
    float birdGravity = BIRD_GRAVITY;
    float obstacleSpeed = OBSTACLE_SPEED;
    int obstacleGap = OBSTACLE_GAP;
    string birdName = BIRD_NAME;

    //exit button
    button_t exitButton("Back", sf::Vector2f(WIN_X_LEN / 8, WIN_Y_LEN / 8));

    //strength setting
    textBox_t strengthText("strength", 30, sf::Vector2f(WIN_X_LEN / 5, WIN_Y_LEN * 0.3));
    textBox_t strengthValueText(to_string(birdJumpSpeed), 20, sf::Vector2f(WIN_X_LEN / 3 * 2, WIN_Y_LEN * 0.3 - 20));
    slider_t strengthSlider(3.0, 20, birdJumpSpeed, 0.1, sf::Vector2f(WIN_X_LEN / 3 * 2, WIN_Y_LEN * 0.3));

    //gravity setting
    textBox_t gravityText("Gravity", 30, sf::Vector2f(WIN_X_LEN / 5, WIN_Y_LEN * 0.4));
    textBox_t gravityValueText(to_string(birdGravity), 20, sf::Vector2f(WIN_X_LEN / 3 * 2, WIN_Y_LEN * 0.4 - 20));
    slider_t gravitySlider(0.1, 1.5, birdGravity, 0.1, sf::Vector2f(WIN_X_LEN / 3 * 2, WIN_Y_LEN * 0.4));

    //speed setting
    textBox_t speedText("Speed", 30, sf::Vector2f(WIN_X_LEN / 5, WIN_Y_LEN * 0.5));
    textBox_t speedValueText(to_string(obstacleSpeed), 20, sf::Vector2f(WIN_X_LEN / 3 * 2, WIN_Y_LEN * 0.5 - 20));
    slider_t speedSlider(2.0, 11.0, obstacleSpeed, 0.1, sf::Vector2f(WIN_X_LEN / 3 * 2, WIN_Y_LEN * 0.5));

    //obstacle gap setting
    textBox_t gapText("Gap", 30, sf::Vector2f(WIN_X_LEN / 5, WIN_Y_LEN * 0.6));
    textBox_t gapValueText(to_string(obstacleGap), 20, sf::Vector2f(WIN_X_LEN / 3 * 2, WIN_Y_LEN * 0.6 - 20));
    slider_t gapSlider(100, WIN_Y_LEN - 200, obstacleGap, 1, sf::Vector2f(WIN_X_LEN / 3 * 2, WIN_Y_LEN * 0.6));

    //bird setting
    textBox_t birdText("Bird", 30, sf::Vector2f(WIN_X_LEN / 5, WIN_Y_LEN * 0.7));
    sf::Sprite birdSprite;
    vector<birdTexture_t> birdTextures;
    int birdIndex = 0;
    fin.open("Images/Birds.txt");
    while (!fin.eof())
    {
        string name;
        getline(fin, name);

        birdTexture_t texture;
        texture.loadFromFile("Images/" + name + "/Idle/0.png");
        texture.name = name;
        birdTextures.push_back(texture);
    }
    fin.close();

    for (unsigned int i = 0; i < birdTextures.size(); i++)
    {
        if (birdTextures[i].name == birdName)
        {
            nextTexture(birdSprite, birdTextures[i]);
            birdIndex = i;
            break;
        }
    }
    birdSprite.setPosition(WIN_X_LEN / 3 * 2, WIN_Y_LEN * 0.7);
    button_t birdLeftButton("<", sf::Vector2f(WIN_X_LEN / 3 * 2 - BIRD_LENGTH, WIN_Y_LEN * 0.7));
    button_t birdRightButton(">", sf::Vector2f(WIN_X_LEN / 3 * 2 + BIRD_LENGTH, WIN_Y_LEN * 0.7));

    //save button
    button_t saveButton("Save", sf::Vector2f(WIN_X_LEN / 4, WIN_Y_LEN * 0.85));

    //reset button
    button_t resetButton("Reset", sf::Vector2f(WIN_X_LEN / 4 * 3, WIN_Y_LEN * 0.85));

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
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                //check if the player has clicked on anything
                if (exitButton.isPressed(window))
                {
                    return screenName_t::home;
                }
                else if (birdLeftButton.isPressed(window))
                {
                    birdIndex = (birdIndex + birdTextures.size() - 1) % birdTextures.size();
                    nextTexture(birdSprite, birdTextures[birdIndex]);
                    birdName = birdTextures[birdIndex].name;
                }
                else if (birdRightButton.isPressed(window))
                {
                    birdIndex = (birdIndex + 1) % birdTextures.size();
                    nextTexture(birdSprite, birdTextures[birdIndex]);
                    birdName = birdTextures[birdIndex].name;
                }
                else if (saveButton.isPressed(window))
                {
                    //save all of the values to the constant variables
                    BIRD_JUMP_SPEED = birdJumpSpeed;
                    BIRD_GRAVITY = birdGravity;
                    OBSTACLE_SPEED = obstacleSpeed;
                    OBSTACLE_GAP = obstacleGap;
                    BIRD_NAME = birdName;
                    //save all of the values to the "custom.txt" file
                    fout.open("Settings/Custom.txt");
                    fout << BIRD_GRAVITY << "\n";
                    fout << BIRD_JUMP_SPEED << "\n";
                    fout << OBSTACLE_SPEED << "\n";
                    fout << OBSTACLE_GAP << "\n";
                    fout << BIRD_NAME << "\n";
                    fout.close();
                    return screenName_t::home;
                }
                else if (resetButton.isPressed(window))
                {
                    //retrieve the default values
                    fin.open("Settings/Default.txt");
                    fin >> birdGravity;
                    fin >> birdJumpSpeed;
                    fin >> obstacleSpeed;
                    fin >> obstacleGap;
                    getline(fin, birdName);
                    getline(fin, birdName); //this code is repeated because fin reads the previous line's newline character and counts it as a line, which gives an empty string
                    fin.close();

                    //apply all the values to the sliders and text
                    strengthSlider.setValue(birdJumpSpeed);
                    strengthValueText.setString(to_string(birdJumpSpeed));

                    gravitySlider.setValue(birdGravity);
                    gravityValueText.setString(to_string(birdGravity));

                    speedSlider.setValue(obstacleSpeed);
                    speedValueText.setString(to_string(obstacleSpeed));

                    gapSlider.setValue(obstacleGap);
                    gapValueText.setString(to_string(obstacleGap));

                    for (unsigned int i = 0; i < birdTextures.size(); i++)
                    {
                        if (birdTextures[i].name == birdName)
                        {
                            nextTexture(birdSprite, birdTextures[i]);
                            birdIndex = i;
                            break;
                        }
                    }
                }
                strengthSlider.checkForClick(window);
                gravitySlider.checkForClick(window);
                speedSlider.checkForClick(window);
                gapSlider.checkForClick(window);
            }
            else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            {
                strengthSlider.isClicked = false;
                gravitySlider.isClicked = false;
                speedSlider.isClicked = false;
                gapSlider.isClicked = false;
            }
        }
        window.clear();

        //handle sliders
        strengthSlider.followMouseIfClicked(window);
        birdJumpSpeed = strengthSlider.getValue();
        strengthValueText.setString(to_string(birdJumpSpeed));

        gravitySlider.followMouseIfClicked(window);
        birdGravity = gravitySlider.getValue();
        gravityValueText.setString(to_string(birdGravity));

        speedSlider.followMouseIfClicked(window);
        obstacleSpeed = speedSlider.getValue();
        speedValueText.setString(to_string(obstacleSpeed));

        gapSlider.followMouseIfClicked(window);
        obstacleGap = gapSlider.getValue();
        gapValueText.setString(to_string(obstacleGap));

        //draw everything
        window.draw(exitButton.graphic);
        window.draw(exitButton.text);

        window.draw(strengthText);
        window.draw(strengthValueText);
        window.draw(strengthSlider.path);
        window.draw(strengthSlider.knob);

        window.draw(gravityText);
        window.draw(gravityValueText);
        window.draw(gravitySlider.path);
        window.draw(gravitySlider.knob);

        window.draw(speedText);
        window.draw(speedValueText);
        window.draw(speedSlider.path);
        window.draw(speedSlider.knob);

        window.draw(gapText);
        window.draw(gapValueText);
        window.draw(gapSlider.path);
        window.draw(gapSlider.knob);

        window.draw(birdText);
        window.draw(birdSprite);
        window.draw(birdLeftButton.graphic);
        window.draw(birdLeftButton.text);
        window.draw(birdRightButton.graphic);
        window.draw(birdRightButton.text);

        window.draw(saveButton.graphic);
        window.draw(saveButton.text);

        window.draw(resetButton.graphic);
        window.draw(resetButton.text);

        window.display();
    }
    return screenName_t::none;
}
