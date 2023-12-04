#include <iostream>
#include <SFML/Graphics.hpp>
#include "utils.hpp"
#include "entity.hpp"

const int WIDTH = 360, HEIGHT = WIDTH * 1.6;
const string TITLE = "Game";

class InputManager
{
public:
    static bool keys[256];
};
bool InputManager::keys[256] = {};
class Bat : public Entity
{
public:
    float speed;
    void move(int dir, float dt)
    {

        this->x += dir * speed * dt;
        if (x + boundingBox.width > WIDTH)
        {
            x = WIDTH - boundingBox.width;
        }
        if (x < 0)
        {
            x = 0;
        }
    }
    void update(float dt) override
    {
        Entity::update(dt);
    }
    Bat(float x, float y, sf::Drawable *drawable, sf::FloatRect boundingBox, float speed)
        : Entity(x, y, drawable, boundingBox), speed(speed)
    {
    }

    Bat(float x, float y, sf::Drawable *drawable, vec2f size, float speed)
        : Entity(x, y, drawable, size), speed(speed)
    {
    }
};
class Ball : public Entity
{
private:
    bool ballUp, ballDown;

public:
    float velX, velY;
    float speedX, speedY;
    void (*winLose)(bool &ballUp, bool &ballDown);
    void update(float dt) override
    {
        this->x += velX * dt;
        this->y += velY * dt;
        checkForWall();
        Entity::update(dt);
    }

    void checkForWall()
    {

        if (x + boundingBox.width > WIDTH)
        {
            velX *= -1;
            x = WIDTH - boundingBox.width - 1;
        }
        if (x < 0)
        {
            velX *= -1;
            x = 1;
        }
        if (y < 0)
        {
            velY *= -1;
            y = 1;
            ballDown = true;
        }
        if (y + boundingBox.height > HEIGHT)
        {
            velY *= -1;
            y = HEIGHT - boundingBox.height - 1;
            ballUp = true;
        }
        winLose(ballUp, ballDown);
    }

    void checkForIntersection(sf::FloatRect b)
    {
        if (boundingBox.intersects(b))
        {
            velY *= -1;
            y += sign(velY) * 1;
            float minAngle = -45, maxAngle = 45;
            float angle = minAngle + fmod(rand(), maxAngle - minAngle);
            velX = std::cos(angle) * speedX;
            velY = sign(velY) * (speedY / 2 + fmod(rand(), speedY * 2));
        }
    }
    Ball(float x, float y, sf::Drawable *drawable, vec2f size, float velX, float velY)
        : Entity(x, y, drawable, size), velX(velX), velY(velY), speedX(velX), speedY(velY)
    {
    }
    Ball(float x, float y, sf::Drawable *drawable, sf::FloatRect boundingBox, float velX, float velY)
        : Entity(x, y, drawable, boundingBox), velX(velX), velY(velY), speedX(velX), speedY(velY)
    {
    }
};

vector<Entity *> entities;
int topPWins = 0;
int bottomPWins = 0;

void loadDrawables();
TT void deleteList(vector<T *> l);
void update(float dt);
void render(sf::RenderTarget &window);
void winLose(bool &ballUp, bool &ballDown);

int main()
{

    loadDrawables();

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), TITLE);
    sf::Clock clock;
    sf::Font font;
    if (!font.loadFromFile("fonts/roboto/Roboto-Light.ttf"))
    {
        window.close();
        return -1;
    };
    sf::Text text;
    text.setFont(font);

    text.setFillColor(sf::Color::White);
    text.setCharacterSize(24);
    text.setPosition(WIDTH / 2, HEIGHT / 2);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == 0)
            {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                InputManager::keys[event.key.code] = true;
            }
            else if (event.type == sf::Event::KeyReleased)
            {
                InputManager::keys[event.key.code] = false;
            }
        }
        auto m = clock.restart();
        float dt = m.asSeconds();

        std::stringstream ss;
        ss << "Player 1 Wins: " << topPWins << '\n';
        ss << "Player 2 Wins: " << bottomPWins;

        text.setString(ss.str());
        auto bounds = text.getLocalBounds();
        text.setOrigin(vec2f{bounds.width, bounds.height} / 2.f);

        update(dt);
        window.clear();
        render(window);
        window.draw(text);
        window.display();
    }

    deleteList(entities);
    return 0;
}

void winLose(bool &ballUp, bool &ballDown)
{
    if (ballUp)
    {
        bottomPWins++;
    }
    if (ballDown)
    {
        topPWins++;
    }
    ballUp = false;
    ballDown = false;
}
void loadDrawables()
{
    {
        float vx = 100, vy = 200;
        auto size = vec2f(8, 8);
        auto pos = vec2f(WIDTH / 2 - size.x / 2, HEIGHT / 2);
        auto ball = new sf::RectangleShape(size);
        auto *o = new Ball(pos.x, pos.y, ball, ball->getLocalBounds(), vx, vy);
        o->winLose = ::winLose;
        entities.push_back(o);
    }
    {
        auto speed = 100;
        auto size = vec2f(60, 10);
        auto pos = vec2f(WIDTH / 2 - size.x / 2, HEIGHT - size.y);
        auto pos2 = vec2f(WIDTH / 2 - size.x / 2, 0);
        auto bat = new sf::RectangleShape(size);
        auto bat2 = new sf::RectangleShape(*bat);
        auto *o = new Bat(pos.x, pos.y, bat, size, speed);
        auto *o2 = new Bat(pos2.x, pos2.y, bat2, size, speed);
        entities.push_back(o);
        entities.push_back(o2);
    }
}

TT void deleteList(vector<T *> l)
{
    for (auto b : l)
    {
        delete b;
    }
    l.clear();
}

void update(float dt)
{
    using k = sf::Keyboard;
    auto keys = InputManager::keys;

    int direction = keys[k::Right] - keys[k::Left];
    ((Bat *)entities[1])->move(direction, dt);
    int direction2 = keys[k::D] - keys[k::A];
    ((Bat *)entities[2])->move(direction2, dt);

    for (int i = 0; i < 2; i++)
    {
        ((Ball *)entities[0])->checkForIntersection(((Bat *)entities[i + 1])->boundingBox);
    }
    for (auto a : entities)
    {
        a->update(dt);
    }
}

void render(sf::RenderTarget &window)
{
    for (auto a : entities)
    {
        window.draw(*(a->drawable), a->transformable.getTransform());
    }
}
