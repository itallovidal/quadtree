#include "include/quadtree.hpp"
#include "include/setup.hpp"
#include "include/simulation.hpp"

#include "cmath"

Boundary::Boundary(sf::Vector2f coordinates, float length) : coordinates(coordinates), length(length)
{
    this->coordinates = coordinates;
    this->length = length;

    this->preview.setSize({this->length, this->length});
    this->preview.setOrigin({this->length / 2, this->length / 2});
    this->preview.setPosition(this->coordinates);

    this->preview.setFillColor(sf::Color::Transparent);
    this->preview.setOutlineColor(sf::Color::White);
    this->preview.setOutlineThickness(1.f);
}

bool Boundary::contains(Particle *p)
{
    float halfSize = this->getLength() / 2;
    float boundaryXLeft = this->coordinates.x - halfSize;
    float boundaryXRight = this->coordinates.x + halfSize;

    float boundaryYTop = this->coordinates.y - halfSize;
    float boundaryYBottom = this->coordinates.y + halfSize;

    return (
        p->getCenterPoint().x <= boundaryXRight &&
        p->getCenterPoint().x >= boundaryXLeft &&
        p->getCenterPoint().y >= boundaryYTop &&
        p->getCenterPoint().y <= boundaryYBottom);
}

sf::RectangleShape Boundary::getPreview()
{
    return this->preview;
}

sf::Vector2f Boundary::getCoordinates()
{
    return this->coordinates;
}

float Boundary::getLength()
{
    return this->length;
}

Quadtree::Quadtree(Boundary boundary, int capacity, sf::RenderWindow &window, Simulation &simulation) : boundary(boundary), capacity(capacity), window(window), topLeft(nullptr), bottomLeft(nullptr), topRight(nullptr), bottomRight(nullptr), isDivided(0), simulation(simulation)
{
}

void Quadtree::debug(sf::RenderWindow &window)
{
    window.draw(this->boundary.getPreview());
}

void Quadtree::collisionDetection()
{

    for (int i = 0; i < this->particles.size(); i++)
    {
        for (int j = i + 1; j < this->particles.size(); j++)
        {
            this->simulation.handleCollision(this->particles[i], this->particles[j]);
        }
    }

    if (this->isDivided)
    {
        this->topLeft->collisionDetection();
        this->topRight->collisionDetection();
        this->bottomLeft->collisionDetection();
        this->bottomRight->collisionDetection();
    }
}

void Quadtree::insert(Particle *particle)
{
    if (!this->boundary.contains(particle))
    {
        return;
    }

    if (this->particles.size() < this->capacity && !this->isDivided)
    {
        this->particles.push_back(particle);
        return;
    }

    if (this->boundary.getLength() <= MIN_QUADTREE_SIZE)
    {
        return;
    }

    if (!this->isDivided)
    {
        this->subdivide();

        for (auto &p : this->particles)
        {
            if (this->topLeft->boundary.contains(p))
                this->topLeft->insert(p);
            else if (this->topRight->boundary.contains(p))
                this->topRight->insert(p);
            else if (this->bottomLeft->boundary.contains(p))
                this->bottomLeft->insert(p);
            else if (this->bottomRight->boundary.contains(p))
                this->bottomRight->insert(p);
        }
        this->particles.clear();
    }

    if (this->topLeft->boundary.contains(particle))
    {
        this->topLeft->insert(particle);
        return;
    }

    if (this->topRight->boundary.contains(particle))
    {
        this->topRight->insert(particle);
        return;
    }

    if (this->bottomLeft->boundary.contains(particle))
    {
        this->bottomLeft->insert(particle);
        return;
    }
    if (this->bottomRight->boundary.contains(particle))
    {
        this->bottomRight->insert(particle);
        return;
    }
}

void Quadtree::subdivide()
{
    // setup
    Boundary topLeftBoundary(
        {this->boundary.getCoordinates().x - (this->boundary.getLength() / 4),
         this->boundary.getCoordinates().y - (this->boundary.getLength() / 4)},
        this->boundary.getLength() / 2);

    Boundary topRightBoundary(
        {this->boundary.getCoordinates().x + (this->boundary.getLength() / 4),
         this->boundary.getCoordinates().y - (this->boundary.getLength() / 4)},
        this->boundary.getLength() / 2);

    Boundary bottomLeftBoundary(
        {this->boundary.getCoordinates().x - (this->boundary.getLength() / 4),
         this->boundary.getCoordinates().y + (this->boundary.getLength() / 4)},
        this->boundary.getLength() / 2);

    Boundary bottomRightBoundary(
        {this->boundary.getCoordinates().x + (this->boundary.getLength() / 4),
         this->boundary.getCoordinates().y + (this->boundary.getLength() / 4)},
        this->boundary.getLength() / 2);

    // creating
    this->topLeft = new Quadtree(topLeftBoundary, this->capacity, this->window, this->simulation);
    this->topRight = new Quadtree(topRightBoundary, this->capacity, this->window, this->simulation);
    this->bottomLeft = new Quadtree(bottomLeftBoundary, this->capacity, this->window, this->simulation);
    this->bottomRight = new Quadtree(bottomRightBoundary, this->capacity, this->window, this->simulation);

    // debug

    if (QUADTREE_DEBUG)
    {
        this->topRight->debug(this->window);
        this->topLeft->debug(this->window);
        this->bottomLeft->debug(this->window);
        this->bottomRight->debug(this->window);
    }

    this->isDivided = 1;
}