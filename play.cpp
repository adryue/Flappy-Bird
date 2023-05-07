#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <math.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "play.h"

using namespace std;

const unsigned int TEXT_SIZE = 60;

const float BIRD_LENGTH = 51;
const float BIRD_WIDTH = 36;
const int BIRD_FLAP_ANIMATION_DURATION = 20;

const int OBSTACLE_WIDTH = 78;
const int OBSTACLE_MIN_LENGTH = 10;
const int OBSTACLE_MAX_LENGTH = 200;
const int OBSTACLE_FREQUENCY = 100;

const int GROUND_HEIGHT = 72;

sf::Texture pipeTexture;

class bird_t
{
public:
    vector<sf::Texture> idleTextures;
    vector<sf::Texture> flapTextures;
    int textureIndex;
    bool animateFlap; //determines if the idle/flap textures are ued
    int flapAnimationTimer; //the number of frames spent using the flap textures
    sf::Sprite sprite;

    sf::SoundBuffer flapSoundBuffer;
    sf::Sound flapSound;


    sf::RectangleShape hitbox;
    sf::Rect<float> collisionBox;
    float speed;

    bird_t()
    {
        //textures
        int numTextures = 0;
        ifstream fin;
        //get the idle frames
        fin.open("Images/" + BIRD_NAME + "/Idle/data.txt");
        fin >> numTextures;
        idleTextures.resize(numTextures);
        for (int i = 0; i < numTextures; i++)
        {
            idleTextures[i].loadFromFile("Images/" + BIRD_NAME + "/Idle/" + to_string(i) + ".png");
        }
        fin.close();
        //get the flapping frames
        fin.open("Images/" + BIRD_NAME + "/Flap/data.txt");
        fin >> numTextures;
        flapTextures.resize(numTextures);
        for (int i = 0; i < numTextures; i++)
        {
            flapTextures[i].loadFromFile("Images/" + BIRD_NAME + "/Flap/" + to_string(i) + ".png");
        }
        fin.close();
        textureIndex = 0;
        animateFlap = false;
        flapAnimationTimer = 0;

        //sprite
        sprite.setTexture(idleTextures[0]);
        sprite.setScale(BIRD_LENGTH / idleTextures[0].getSize().x, BIRD_WIDTH / idleTextures[0].getSize().y);
        sprite.setOrigin(idleTextures[0].getSize().x / 2, idleTextures[0].getSize().y / 2);
        sprite.setPosition(WIN_X_LEN / 8, WIN_Y_LEN / 2);

        //sound
        flapSoundBuffer.loadFromFile("Sounds/Flap.wav");
        flapSound.setBuffer(flapSoundBuffer);

        //hitox (viewable rectangle)
        hitbox.setFillColor(sf::Color::Red);
        hitbox.setSize(sf::Vector2f(BIRD_LENGTH, BIRD_WIDTH));
        hitbox.setOrigin(BIRD_LENGTH / 2, BIRD_WIDTH / 2);
        hitbox.setPosition(WIN_X_LEN / 8, WIN_Y_LEN / 2);

        //collisionBox (sf::Rect to check for collisions)
        collisionBox.left = WIN_X_LEN / 8 - (BIRD_LENGTH / 2);
        collisionBox.top = (WIN_Y_LEN / 2) - (BIRD_WIDTH / 2);
        collisionBox.width = BIRD_LENGTH;
        collisionBox.height = BIRD_WIDTH;

        speed = 0;
    }
    void angle()
    {
        sprite.setRotation(atan(speed / OBSTACLE_SPEED) * 180 / M_PI);
    }
    void flap()
    {
        speed = -BIRD_JUMP_SPEED;
        animateFlap = true;
        flapAnimationTimer = 0;
        textureIndex = 0;

        flapSound.play();
    }
    void animate()
    {
        vector<sf::Texture> &textures = animateFlap ? flapTextures : idleTextures;
        textureIndex = (textureIndex + 1) % textures.size();
        sprite.setTexture(textures[textureIndex]);
        sprite.setScale(BIRD_LENGTH / textures[textureIndex].getSize().x, BIRD_WIDTH / textures[textureIndex].getSize().y);
        sprite.setOrigin(textures[textureIndex].getSize().x / 2, textures[textureIndex].getSize().y / 2);

        if (animateFlap)
        {
            if (++flapAnimationTimer >= BIRD_FLAP_ANIMATION_DURATION)
            {
                animateFlap = false;
                textureIndex = 0;
            }
        }
    }
};

class obstacle_t
{
public:
    sf::RectangleShape top;
    sf::RectangleShape bottom;

    sf::Sprite topSprite;
    sf::Sprite bottomSprite;

    sf::Rect<float> topCollisionBox;
    sf::Rect<float> bottomCollisionBox;

    obstacle_t(int topLength)
    {
        top.setSize(sf::Vector2f(OBSTACLE_WIDTH, topLength));
        top.setPosition(WIN_X_LEN, 0.0);
        topCollisionBox.left = WIN_X_LEN;
        topCollisionBox.top = 0.0;
        topCollisionBox.width = OBSTACLE_WIDTH;
        topCollisionBox.height = topLength;

        bottom.setSize(sf::Vector2f(OBSTACLE_WIDTH, WIN_Y_LEN - (topLength + OBSTACLE_GAP)));
        bottom.setPosition(WIN_X_LEN, topLength + OBSTACLE_GAP);
        bottomCollisionBox.left = WIN_X_LEN;
        bottomCollisionBox.top = topLength + OBSTACLE_GAP;
        bottomCollisionBox.width = OBSTACLE_WIDTH;
        bottomCollisionBox.height = WIN_Y_LEN - (topLength + OBSTACLE_GAP);

        topSprite.setTexture(pipeTexture);
        topSprite.setScale(OBSTACLE_WIDTH / pipeTexture.getSize().x, 1);
        topSprite.setOrigin(0, pipeTexture.getSize().y);
        topSprite.setPosition(WIN_X_LEN, topLength);

        bottomSprite.setTexture(pipeTexture);
        bottomSprite.setScale(OBSTACLE_WIDTH / pipeTexture.getSize().x, 1);
        bottomSprite.setPosition(WIN_X_LEN, topLength + OBSTACLE_GAP);
    }
    void update()
    {
        top.move(-OBSTACLE_SPEED, 0.0);
        topCollisionBox.left = topCollisionBox.left - OBSTACLE_SPEED;
        topSprite.move(-OBSTACLE_SPEED, 0.0);

        bottom.move(-OBSTACLE_SPEED, 0.0);
        bottomCollisionBox.left = bottomCollisionBox.left - OBSTACLE_SPEED;
        bottomSprite.move(-OBSTACLE_SPEED, 0.0);
    }
};

class background_t
{
public:
    sf::Texture unitTexture;
    sf::Sprite unit;
    float initialPosition;
    float speed; //speed at which the background moves

    background_t(string textureName, float shiftSpeed, float offset)
    {
        unitTexture.loadFromFile("Images/Backgrounds/" + textureName + ".png");
        //unitTexture.loadFromFile("Images/0.png");
        unit.setTexture(unitTexture);
        unit.setOrigin(0, unit.getLocalBounds().height); //bottom left
        unit.setPosition(0, WIN_Y_LEN - offset);

        initialPosition = 0.0;
        speed = shiftSpeed;
    }
    void draw(sf::RenderWindow &window)
    {
        unit.setPosition(initialPosition, unit.getPosition().y);
        while (unit.getPosition().x < WIN_X_LEN)
        {
            window.draw(unit);
            unit.move(unit.getLocalBounds().width, 0);
        }
    }
    void shift()
    {
        initialPosition -= speed;
        if (-initialPosition > unit.getLocalBounds().width)
        {
            //prevent unnecessary sprites from being drawn to the left of the screen
            initialPosition += unit.getLocalBounds().width;
        }
    }
};

screenName_t playScreen(sf::RenderWindow &window)
{
    srand(time(NULL));

    score = 0;

    //bird
    bird_t bird;
    bool singlePress = true;

    //obstacles
    pipeTexture.loadFromFile("Images/Pipe.png");
    vector<obstacle_t> obstacles;
    obstacles.push_back(obstacle_t((rand() % (OBSTACLE_MAX_LENGTH - OBSTACLE_MIN_LENGTH)) + OBSTACLE_MIN_LENGTH));
    int obstacleTimer = 0; //the number of frames since the last obstacle was spawned

    //score
    sf::Text scoreText("0", font, TEXT_SIZE);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(scoreText.getLocalBounds().top, 0);

    //backgrounds
    background_t treesBackground("Trees", OBSTACLE_SPEED * 0.8, GROUND_HEIGHT);
    background_t buildingsBackground("Buildings", OBSTACLE_SPEED * 0.5, 104);
    background_t cloudsBackground("Clouds", OBSTACLE_SPEED * 0.1, 0);
    background_t ground("Ground", OBSTACLE_SPEED, 0);

    //sounds
    sf::SoundBuffer scoreSoundBuffer;
    scoreSoundBuffer.loadFromFile("Sounds/Score.wav");
    sf::Sound scoreSound(scoreSoundBuffer);

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
        }
        window.clear();

        //handle player input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            if (singlePress)
            {
                singlePress = false;
                bird.flap();
            }
        }
        else
        {
            singlePress = true;
        }

        //handle physics
        //player
        bird.animate();
        bird.speed += BIRD_GRAVITY;
        bird.hitbox.move(0, bird.speed);
        bird.collisionBox.top = bird.collisionBox.top + bird.speed;
        if (bird.collisionBox.top < 0)
        {
            //prevent the bird from going over the obstacles
            bird.hitbox.setPosition(bird.hitbox.getPosition().x, BIRD_WIDTH / 2);
            bird.collisionBox.top = 0;
        }

        //obstacles
        //spawn a new obstacle every [OBSTACLE_FREQUENCY] frames
        obstacleTimer++;
        if (obstacleTimer >= OBSTACLE_FREQUENCY)
        {
            obstacleTimer = 0;
            obstacles.push_back(obstacle_t((rand() % (OBSTACLE_MAX_LENGTH - OBSTACLE_MIN_LENGTH)) + OBSTACLE_MIN_LENGTH));
        }
        //scroll all the obstacles left
        for (obstacle_t &o : obstacles)
        {
            o.update();
        }
        //delete obstacles that scroll past the screen
        if (!obstacles.empty())
        {
            if (obstacles[0].topCollisionBox.left + obstacles[0].topCollisionBox.width < 0)
            {
                obstacles.erase(obstacles.begin());
            }
        }

        //update the score if needed
        for (obstacle_t &o : obstacles)
        {
            if (signbit((o.topCollisionBox.left + o.topCollisionBox.width) - bird.collisionBox.left) !=
                signbit((o.topCollisionBox.left + o.topCollisionBox.width) - bird.collisionBox.left + OBSTACLE_SPEED)) //the bird is behind the obstacle in one frame and in front in the next frame
            {
                score++;
                scoreText.setString(to_string(score));
                //scoreSound.play();
                break;
            }
        }

        //check if the player has died
        if (bird.collisionBox.top + bird.collisionBox.height > WIN_Y_LEN - GROUND_HEIGHT) //fell to the floor
        {
            return screenName_t::lose;
        }
        for (obstacle_t &o : obstacles)
        {
            if (bird.collisionBox.intersects(o.topCollisionBox) || bird.collisionBox.intersects(o.bottomCollisionBox)) //hits an obstacle
            {
                return screenName_t::lose;
            }
        }

        //move the backgrounds
        treesBackground.shift();
        buildingsBackground.shift();
        cloudsBackground.shift();
        ground.shift();

        //draw everything
        cloudsBackground.draw(window);
        buildingsBackground.draw(window);
        treesBackground.draw(window);
        for (obstacle_t &o : obstacles)
        {
            //window.draw(o.top);
            window.draw(o.topSprite);
            //window.draw(o.bottom);
            window.draw(o.bottomSprite);
        }
        ground.draw(window);
        bird.sprite.setPosition(bird.hitbox.getPosition());
        bird.angle();
        //window.draw(bird.hitbox);
        window.draw(bird.sprite);
        window.draw(scoreText);

        window.display();
    }
    return screenName_t::none;
}
