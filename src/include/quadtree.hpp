#ifndef QUADTREE
#define QUADTREE

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "particle.hpp"
#include "simulation.hpp"
#include <vector>

class Simulation;

class Boundary
{
private:
    sf::Vector2f coordinates;
    float length;
    sf::RectangleShape preview;

public:
    Boundary(sf::Vector2f coordinates, float length);
    bool contains(Particle p);
    sf::RectangleShape getPreview();
    sf::Vector2f getCoordinates();
    float getLength();
};

class Quadtree
{
private:
    Boundary boundary;
    int capacity;
    Quadtree *topLeft;
    Quadtree *bottomLeft;
    Quadtree *topRight;
    Quadtree *bottomRight;
    sf::RenderWindow &window;
    std::vector<Particle> particles;
    int isDivided = 0;
    Simulation &simulation;

public:
    Quadtree(Boundary boundary, int capacity, sf::RenderWindow &window, Simulation &simulation);
    void insert(Particle particle);
    void collisionDetection();
    void debug(sf::RenderWindow &window);
    void subdivide();
};

#endif