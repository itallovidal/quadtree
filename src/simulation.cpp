#include "include/simulation.hpp"
#include "include/setup.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

// Implementação dos métodos da classe Simulation

Simulation::Simulation(sf::RenderWindow &window) : window(window)
{
    for (int i = 0; i < BALL_COUNT; i++)
    {
        Particle p;
        this->particles.push_back(p);
    }
}

void Simulation::update(float deltaTime)
{
    Boundary boundary({WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2}, WINDOW_HEIGHT);
    Quadtree initialQuadtree(boundary, MAX_QUADTREE_PARTICLES, this->window, *this);

    for (int i = 0; i < this->particles.size(); i++)
    {
        this->window.draw(this->particles[i].spawn(deltaTime));
        initialQuadtree.insert(&particles[i]);
    }

    this->HandleQuadtreeCollision(&initialQuadtree);
}

void Simulation::HandleQuadtreeCollision(Quadtree *quadtree)
{
    std::vector<Particle *> particles = quadtree->getParticles();
    for (int i = 0; i < particles.size(); i++)
    {
        for (int j = i + 1; j < particles.size(); j++)
        {
            this->handleCollision(particles[i], particles[j]);
        }
    }

    if (quadtree->getIsDivided())
    {
        this->HandleQuadtreeCollision(quadtree->getSubtree(TREE_LOCATION::BOTTOM_LEFT));
        this->HandleQuadtreeCollision(quadtree->getSubtree(TREE_LOCATION::BOTTOM_RIGHT));
        this->HandleQuadtreeCollision(quadtree->getSubtree(TREE_LOCATION::TOP_LEFT));
        this->HandleQuadtreeCollision(quadtree->getSubtree(TREE_LOCATION::TOP_RIGHT));
    }
}

void Simulation::handleCollision(Particle *particleA, Particle *particleB)
{
    float xDistance = abs(particleA->getCenterPoint().x - particleB->getCenterPoint().x);
    float yDistance = abs(particleA->getCenterPoint().y - particleB->getCenterPoint().y);

    // a² + b² = c²
    float h = (xDistance * xDistance) + (yDistance * yDistance);
    float distance = sqrt(h);

    float sumOfRadius = particleA->radius() + particleB->radius();

    if (distance <= sumOfRadius)
    {
        this->handleImpact(particleA, particleB, distance);
    }
}

void Simulation::handleImpact(Particle *particleA, Particle *particleB, float distance)
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

void Simulation::handleOverlap(Particle *particleA, Particle *particleB, float distance)
{
    float idealDistance = particleA->radius() + particleB->radius();

    if (distance >= idealDistance)
    {
        return;
    }

    float overlap = (idealDistance - distance);

    sf::Vector2f direction = particleB->getCenterPoint() - particleA->getCenterPoint();
    sf::Vector2f normalizedDirection = direction / (distance > 0 ? distance : 1);
    sf::Vector2f pushBack = normalizedDirection * (overlap / 2);

    particleA->setPosition(particleA->getCenterPoint() - pushBack);
    particleB->setPosition(particleB->getCenterPoint() + pushBack);
    return;
}