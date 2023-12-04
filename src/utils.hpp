#pragma once

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <sstream>
#include <cmath>

#define TT template <typename T>

using sf::Drawable;
using std::string;
using std::vector;

typedef sf::Vector2f vec2f;
typedef sf::Vector2i vec2i;

TT void print(T a, char end = '\n');
float sign(float a);
float distanceFromEdge(vec2f point, vec2f a, vec2f b);
float distanceFromEdge(vec2f point, vec2f a, float angle);
float min(vector<float> l);
TT void print(T a, char end)
{
    std::cout << a << end;
}

float sign(float a)
{
    if (a > 0)
        return 1;
    if (a < 0)
        return -1;
    return 0;
}

float distanceFromEdge(vec2f point, vec2f a, vec2f b)
{
    float distance;
    distance = std::abs(((b.x - a.x) * (a.y - point.y) - (a.x - point.x) * (b.y - a.y)) /
                        (std::sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y))));
    return distance;
}
float distanceFromEdge(vec2f point, vec2f a, float angle)
{
    float distance;
    distance = std::abs(std::cos(angle) * (a.y - point.y) - std::sin(angle) * (a.x - point.x));
    return distance;
}

inline float min(vector<float> l)
{
    if (l.size() == 0)
        return 0;
    float distance=l[0];
    for (int i = 1; i < l.size(); i++)
    {
        if(distance>l[i]){
            distance=l[i];
        }
    }
    return distance;
}
