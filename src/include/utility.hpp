#ifndef UTILITY
#define UTILITY

#include <random>
#include <SFML/Graphics.hpp>

extern std::mt19937 gen;

float getRandomNumber(int min, int max);

sf::Font loadFont();

class FPSCounter
{
private:
    float fps;
    sf::Text text;

public:
    FPSCounter(sf::Text text);
    void update(float deltaTime);
    void showFPS(sf::RenderWindow &window);
};

#endif