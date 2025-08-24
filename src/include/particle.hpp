#ifndef PARTICLE
#define PARTICLE

#include <SFML/Graphics.hpp>

class Particle

{
private:
    sf::CircleShape shape;
    float mass;
    sf::Vector2f velocity;
    bool isSpawned;
    sf::CircleShape move(float deltaTime);
    sf::Vector2f handleTunneling(sf::Vector2f coordenate);

public:
    Particle();
    sf::Vector2f getCenterPoint();
    float radius();
    float getMass();
    sf::Vector2f getVelocity();
    void setVelocity(sf::Vector2f v);
    void setPosition(sf::Vector2f pos);
    sf::CircleShape spawn(float deltaTime);
};

#endif