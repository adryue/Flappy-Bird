#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <math.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "home.h"
#include "play.h"
#include "lose.h"
#include "settings.h"
#include "commonDef.h"

using namespace std;

const int WIN_X_LEN = 800;
const int WIN_Y_LEN = 600;

const int FRAMERATE = 60;

float BIRD_GRAVITY = 0.35;
float BIRD_JUMP_SPEED = 9.5;
float OBSTACLE_SPEED = 4.0;
int OBSTACLE_GAP = 300;
string BIRD_NAME = "Blue Bird";

int score = 0;
int highScore = 0;

sf::Font font;

int main()
{
    ifstream fin;
    fin.open("Settings/Custom.txt");
    //read all of the custom saved settings
    fin >> BIRD_GRAVITY;
    fin >> BIRD_JUMP_SPEED;
    fin >> OBSTACLE_SPEED;
    fin >> OBSTACLE_GAP;
    getline(fin, BIRD_NAME);
    getline(fin, BIRD_NAME); //this code is repeated because fin reads the previous line's newline character and counts it as a line, which gives an empty string
    fin.close();

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(WIN_X_LEN, WIN_Y_LEN), "SFML window");
    window.setFramerateLimit(FRAMERATE);

    font.loadFromFile("Hyperspace-JvEM.ttf");
    //font.loadFromFile("7LED.ttf");

    screenName_t nextScreen = screenName_t::home;
    bool exit = false;
    while (!exit)
    {
        //choose the next screen to switch to
        switch(nextScreen)
        {
        case home:
            nextScreen = homeScreen(window);
            break;
        case play:
            nextScreen = playScreen(window);
            break;
        case lose:
            nextScreen = loseScreen(window);
            break;
        case settings:
            nextScreen = settingsScreen(window);
            break;
        case none:
            exit = true;
            break;
        default:
            cout << "Invalid screen name" << endl;
            exit = true;
        }
    }

    return EXIT_SUCCESS;
}
