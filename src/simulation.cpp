#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "include/setup.hpp"

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
        Boundary boundary({WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2}, WINDOW_HEIGHT);
        Quadtree initialQuadtree(boundary, MAX_QUADTREE_PARTICLES, this->window);

        for (int i = 0; i < this->particles.size(); i++)
        {
            this->window.draw(this->particles[i].spawn(deltaTime));
            initialQuadtree.insert(particles[i]);
        }

        initialQuadtree.collisionDetection();

        initialQuadtree.debug(window);
    }

    static void handleCollision(Particle *particleA, Particle *particleB)
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
            Simulation::handleImpact(particleA, particleB, distance);
        }
    }

    static void handleImpact(Particle *particleA, Particle *particleB, float distance)
    {
        Simulation::handleOverlap(particleA, particleB, distance);

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

    static void handleOverlap(Particle *particleA, Particle *particleB, float distance)
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