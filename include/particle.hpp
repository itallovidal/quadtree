#pragma once
#include <SFML/Graphics.hpp>

class Particle
{
public:
    Particle();
    sf::Vector2f getCenterPoint() const;
    float radius() const;
    float getMass() const;
    sf::Vector2f getVelocity() const;
    void setVelocity(const sf::Vector2f v);
    void setPosition(const sf::Vector2f pos);
    sf::CircleShape spawn(float deltaTime);
};