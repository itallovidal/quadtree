#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <random>
#include <time.h>
#include <cmath>
#include "utility.cpp"

#define BALL_COUNT 100
#define MASS_MIN_NUMBER 100.f
#define MASS_MAX_NUMBER 120.f

class Particle
{
    float mass;
    sf::Vector2f velocity = {
        120.f, 120.f};
    int isSpawned = 0;

    sf::CircleShape shape;

public:
    Particle()
    {
        this->shape.setFillColor(sf::Color::White);
        this->mass = getRandomNumber(MASS_MIN_NUMBER, MASS_MAX_NUMBER);
        float radius = sqrt(this->mass) * 3.f;
        this->shape.setOrigin({radius, radius});
        this->shape.setRadius(radius);
    }

    float getMass()
    {
        return this->mass;
    }

    sf::Vector2f getVelocity()
    {
        return this->velocity;
    }

    float radius()
    {
        return this->shape.getRadius();
    }

    sf::CircleShape spawn(float deltaTime)
    {
        if (!this->isSpawned)
        {
            float x = getRandomNumber(0, WINDOW_WIDTH - this->radius() * 2);
            float y = getRandomNumber(0, WINDOW_HEIGHT - this->radius() * 2);
            this->shape.setPosition({x, y});
            this->isSpawned = 1;
            return this->shape;
        }

        return this->move(deltaTime);
    }

    sf::CircleShape move(float deltaTime)
    {
        // S = S₀ + v ⋅ t
        float x = this->shape.getPosition().x + this->velocity.x * deltaTime;
        float y = this->shape.getPosition().y + this->velocity.y * deltaTime;

        // handle tunneling
        sf::Vector2f newPosition = this->handleTunneling({x, y});

        this->shape.setPosition(newPosition);
        return this->shape;
    }

    sf::Vector2f getCenterPoint()
    {
        sf::Vector2f points =
            {this->shape.getPosition().x,
             this->shape.getPosition().y};

        return points;
    }

    void setPosition(sf::Vector2f newPosition)
    {
        this->shape.setPosition(newPosition);
    }

    void setVelocity(sf::Vector2f newVelocity)
    {
        this->velocity = newVelocity;
    }

    sf::Vector2f handleTunneling(sf::Vector2f coordenate)
    {
        if (coordenate.x + this->radius() > WINDOW_WIDTH)
        {
            this->velocity.x *= -1;
            float overlap = coordenate.x + this->radius() - WINDOW_WIDTH;

            coordenate.x -= overlap;
        }

        if (coordenate.x - this->radius() < 0)
        {
            this->velocity.x *= -1;
            float overlap = abs((coordenate.x - this->radius()) + 0);

            coordenate.x += overlap;
        }

        if (coordenate.y + this->radius() > WINDOW_HEIGHT)
        {
            this->velocity.y *= -1;
            float overlap = coordenate.y + this->radius() - WINDOW_HEIGHT;

            coordenate.y -= overlap;
        }

        if (coordenate.y - this->radius() < 0)
        {
            this->velocity.y *= -1;
            float overlap = abs((coordenate.y - this->radius()) + 0);

            coordenate.y += overlap;
        }

        return coordenate;
    }
};

class Simulation
{
    sf::RenderWindow &window;
    std::vector<Particle> particles;

public:
    Simulation(sf::RenderWindow &window) : window(window)
    {
        for (int i = 0; i < BALL_COUNT; i++)
        {
            Particle p;
            this->particles.push_back(p);
        }
    }

    void update(float deltaTime)
    {
        for (int i = 0; i < this->particles.size(); i++)
        {
            this->window.draw(this->particles[i].spawn(deltaTime));

            for (int j = i + 1; j < this->particles.size(); j++)
            {
                this->handleCollision(&this->particles[i], &this->particles[j]);
            }
        }
    }

    void handleCollision(Particle *particleA, Particle *particleB)
    {
        float xDistance = abs(particleA->getCenterPoint().x - particleB->getCenterPoint().x);
        float yDistance = abs(particleA->getCenterPoint().y - particleB->getCenterPoint().y);

        // a² + b² = c²
        float h = (xDistance * xDistance) + (yDistance * yDistance);
        float distance = sqrt(h);

        float sumOfRadius = particleA->radius() + particleB->radius();

        if (distance <= sumOfRadius)
        {
            std::cout << "Collision Ocurred!" << "\n";
            this->handleImpact(particleA, particleB, distance);
        }
    }

    void handleImpact(Particle *particleA, Particle *particleB, float distance)
    {
        this->handleOverlap(particleA, particleB, distance);

        // Particle 1
        float massFactor = 2.f * particleB->getMass() / (particleA->getMass() + particleB->getMass());
        sf::Vector2f relativeVelocity = particleB->getVelocity() - particleA->getVelocity();
        sf::Vector2f lineOfImpact = particleB->getCenterPoint() - particleA->getCenterPoint();

        float scalarProjectionNumerator = (relativeVelocity.x * lineOfImpact.x) + (relativeVelocity.y * lineOfImpact.y);
        float scalarProjectionDenominator = (lineOfImpact.x * lineOfImpact.x) + (lineOfImpact.y * lineOfImpact.y);

        float scalarProjection = scalarProjectionNumerator / scalarProjectionDenominator;
        sf::Vector2f v1Prime = particleA->getVelocity() + massFactor * scalarProjection * lineOfImpact;

        // Particle 2
        float massFactor2 = 2.f * particleA->getMass() / (particleA->getMass() + particleB->getMass());
        sf::Vector2f relativeVelocity2 = particleA->getVelocity() - particleB->getVelocity();
        sf::Vector2f lineOfImpact2 = particleA->getCenterPoint() - particleB->getCenterPoint();

        float scalarProjectionNumerator2 = (relativeVelocity2.x * lineOfImpact2.x) + (relativeVelocity2.y * lineOfImpact2.y);
        float scalarProjectionDenominator2 = (lineOfImpact2.x * lineOfImpact2.x) + (lineOfImpact2.y * lineOfImpact2.y);

        float scalarProjection2 = scalarProjectionNumerator2 / scalarProjectionDenominator2;
        sf::Vector2f v2Prime = particleB->getVelocity() + massFactor2 * scalarProjection2 * lineOfImpact2;

        particleA->setVelocity(v1Prime);
        particleB->setVelocity(v2Prime);
    }

    void handleOverlap(Particle *particleA, Particle *particleB, float distance)
    {
        float idealDistance = particleA->radius() + particleB->radius();

        if (distance >= idealDistance)
        {
            return;
        }

        std::cout << "Overlaping!" << "\n";
        float overlap = (idealDistance - distance);

        sf::Vector2f direction = particleB->getCenterPoint() - particleA->getCenterPoint();
        sf::Vector2f normalizedDirection = direction / (distance > 0 ? distance : 1);
        sf::Vector2f pushBack = normalizedDirection * (overlap / 2);

        particleA->setPosition(particleA->getCenterPoint() - pushBack);
        particleB->setPosition(particleB->getCenterPoint() + pushBack);
        return;
    }
};