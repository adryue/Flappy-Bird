#ifndef COMMONDEF_H_INCLUDED
#define COMMONDEF_H_INCLUDED

#include <string>
#include <SFML/Graphics.hpp>
enum screenName_t
{
    none,
    home,
    play,
    lose,
    settings
};

extern const int WIN_X_LEN;
extern const int WIN_Y_LEN;
extern const int FRAMERATE;

extern const float BIRD_LENGTH;
extern const float BIRD_WIDTH;
extern float BIRD_GRAVITY;
extern float BIRD_JUMP_SPEED;
extern float OBSTACLE_SPEED;
extern int OBSTACLE_GAP;
extern std::string BIRD_NAME;

extern int score;
extern int highScore;

extern sf::Font font;

#endif // COMMONDEF_H_INCLUDED
