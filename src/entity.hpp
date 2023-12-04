#include "utils.hpp"
#include <SFML/Graphics/Drawable.hpp>

class Entity
{
public:
    float x, y;
    sf::Drawable *drawable;
    sf::FloatRect boundingBox;
    sf::Transformable transformable;
    virtual void update(float dt)
    {
        transformable.setPosition(x, y);
        boundingBox.left = x;
        boundingBox.top = y;
        
    }
    Entity(float x, float y, sf::Drawable *drawable, vec2f size)
        : x(x), y(y), drawable(drawable)
    {
        boundingBox = sf::FloatRect(x, y, size.x, size.y);
    }
    Entity(float x, float y, sf::Drawable *drawable, sf::FloatRect boundingBox)
        : x(x), y(y), boundingBox(boundingBox), drawable(drawable)
    {
    }
    ~Entity()
    {
        delete drawable;
    }
};
