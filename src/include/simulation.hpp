#ifndef SIMULATION
#define SIMULATION

#include <SFML/Graphics.hpp>
#include <vector>
#include "setup.hpp"
#include "particle.hpp"
#include "quadtree.hpp"

class Quadtree;

class Simulation
{
    sf::RenderWindow &window;
    std::vector<Particle> particles;

public:
    Simulation(sf::RenderWindow &window);

    void update(float deltaTime);

    void HandleQuadtreeCollision(Quadtree *quadtree);
    void handleCollision(Particle *particleA, Particle *particleB);
    void handleImpact(Particle *particleA, Particle *particleB, float distance);
    void handleOverlap(Particle *particleA, Particle *particleB, float distance);
};

#endif