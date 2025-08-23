#include "utility.cpp"

class Boundary
{
    sf::Vector2f coordinates;
    float length;
    sf::RectangleShape preview;

public:
    Boundary(sf::Vector2f coordinates, float length)
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

    int contains(Particle p)
    {
        float halfSize = (this->getLength() / 2);
        float boundaryXLeft = this->coordinates.x - halfSize;
        float boundaryXRight = this->coordinates.x + halfSize;

        float boundaryYTop = this->coordinates.y - halfSize;
        float boundaryYBottom = this->coordinates.y + halfSize;

        if (p.getCenterPoint().x < boundaryXLeft)
        {
            std::cout << "a" << "\n";
            return 0;
        }

        if (p.getCenterPoint().x > boundaryXRight)
        {
            std::cout << "b" << "\n";
            return 0;
        }

        if (p.getCenterPoint().y < boundaryYTop)
        {
            std::cout << "c" << "\n";
            return 0;
        }

        if (p.getCenterPoint().y > boundaryYBottom)
        {
            std::cout << "d" << "\n";
            return 0;
        }

        return 1;
    }

    sf::RectangleShape getPreview()
    {
        return this->preview;
    }

    sf::Vector2f getCoordinates()
    {
        return this->coordinates;
    }

    float getLength()
    {
        return this->length;
    }
};

class Quadtree
{
    Boundary boundary;
    int capacity;
    Quadtree *topLeft;
    Quadtree *bottomLeft;
    Quadtree *topRight;
    Quadtree *bottomRight;
    sf::RenderWindow &window;
    std::vector<Particle> particles;

    int isDivided = 0;

public:
    Quadtree(Boundary boundary, int capacity, sf::RenderWindow &window) : boundary(boundary), window(window)
    {
        this->capacity = capacity;
        this->boundary = boundary;
    }

    void debug(sf::RenderWindow &window)
    {
        window.draw(this->boundary.getPreview());
    }

    void
    collisionDetection()
    {
        if (this->particles.size() < 2)
        {
            return;
        }

        for (int i = 0; i < this->particles.size() - 1; i++)
        {
            Simulation::handleCollision(&this->particles[i], &this->particles[i + 1]);
        }

        if (this->isDivided)
        {
            this->topLeft->collisionDetection();
            this->topRight->collisionDetection();
            this->bottomLeft->collisionDetection();
            this->bottomRight->collisionDetection();
        }
    }

    void insert(Particle particle)
    {
        std::cout << this->particles.size() << "\n";
        if (!this->boundary.contains(particle))
        {
            std::cout << "não contem" << "\n";
            return;
        }

        if (this->particles.size() < this->capacity)
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
        }

        this->topLeft->insert(particle);
        this->topRight->insert(particle);

        this->bottomLeft->insert(particle);
        this->bottomRight->insert(particle);
    }

    void subdivide()
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
            {this->boundary.getCoordinates().x / 2,
             this->boundary.getCoordinates().y + (this->boundary.getLength() / 4)},
            this->boundary.getLength() / 2);

        Boundary bottomRightBoundary(
            {this->boundary.getCoordinates().x + (this->boundary.getLength() / 4),
             this->boundary.getCoordinates().y + (this->boundary.getLength() / 4)},
            this->boundary.getLength() / 2);

        // creating
        this->topLeft = new Quadtree(topLeftBoundary, this->capacity, this->window);
        this->topRight = new Quadtree(topRightBoundary, this->capacity, this->window);
        this->bottomLeft = new Quadtree(bottomLeftBoundary, this->capacity, this->window);
        this->bottomRight = new Quadtree(bottomRightBoundary, this->capacity, this->window);

        // debug
        this->topRight->debug(this->window);
        this->topLeft->debug(this->window);
        this->bottomLeft->debug(this->window);
        this->bottomRight->debug(this->window);

        this->isDivided = 1;
    }
};