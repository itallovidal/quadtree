#include "utility.cpp"

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