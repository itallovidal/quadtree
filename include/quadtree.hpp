#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "particle.hpp"
#include "utility.hpp"

class Quadtree
{
public:
    Quadtree(const Boundary boundary, int capacity, sf::RenderWindow &window);
    void insert(const Particle particle);
    void collisionDetection();
    void debug(sf::RenderWindow &window);
};