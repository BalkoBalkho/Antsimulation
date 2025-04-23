#include "Ant.h"
#include <cmath>
#include <cstdlib>
#include <algorithm>

Ant::Ant(sf::Vector2f pos)
    : position(pos), speed(ANT_SPEED), hasFood(false),
      legAngleOffset(0), animationTimer(0), bodyRotation(0)
{

    float angle = static_cast<float>(std::rand() % 360) * 3.14159f / 180.0f;
    direction = sf::Vector2f(std::cos(angle), std::sin(angle));

    body.setPointCount(6);
    body.setPoint(0, sf::Vector2f(0, -5));
    body.setPoint(1, sf::Vector2f(10, -3));
    body.setPoint(2, sf::Vector2f(15, 0));
    body.setPoint(3, sf::Vector2f(10, 3));
    body.setPoint(4, sf::Vector2f(0, 5));
    body.setPoint(5, sf::Vector2f(-5, 0));
    body.setFillColor(ANT_BODY_COLOR);
    body.setOutlineColor(sf::Color::Black);
    body.setOutlineThickness(1);
    body.setOrigin(7.5f, 0);

    head.setRadius(6);
    head.setFillColor(ANT_HEAD_COLOR);
    head.setOutlineColor(sf::Color::Black);
    head.setOutlineThickness(1);
    head.setOrigin(6, 6);

    for (int i = 0; i < 6; ++i)
    {
        sf::RectangleShape leg(sf::Vector2f(12, 2));
        leg.setFillColor(ANT_LEG_COLOR);
        leg.setOrigin(0, 1);
        legs.push_back(leg);
    }

    foodIndicator.setRadius(3);
    foodIndicator.setFillColor(sf::Color::Yellow);
    foodIndicator.setOrigin(3, 3);
}

void Ant::update(float dt, std::vector<Pheromone> &pheromones, const std::vector<Food> &foods, const sf::Vector2f &nestPos)
{
    if (std::rand() % 100 < 5)
    {
        float angle = static_cast<float>(std::rand() % 60 - 30) * 3.14159f / 180.0f;
        rotate(angle);
    }

    // Follow pheromone
    Pheromone *strongest = nullptr;
    float maxStrength = 0.0f;
    for (auto &p : pheromones)
    {
        float dist = hypot(p.getPosition().x - position.x, p.getPosition().y - position.y);
        if (dist < 40.0f && p.hasFood != hasFood && p.strength > maxStrength)
        {
            maxStrength = p.strength;
            strongest = &p;
        }
    }

    if (strongest && maxStrength > 50.0f) { // Threshold for pheromone strength
        sf::Vector2f dirToPheromone = strongest->getPosition() - position;
        float len = sqrt(dirToPheromone.x * dirToPheromone.x + dirToPheromone.y * dirToPheromone.y);
        if (len > 0)
            direction = dirToPheromone / len;
    } else {
        // If no strong pheromone is found, move randomly
        if (std::rand() % 100 < 10) { // 10% chance to change direction randomly
            float angle = static_cast<float>(std::rand() % 360) * 3.14159f / 180.0f;
            direction = sf::Vector2f(std::cos(angle), std::sin(angle));
        }
    }

    // Move
    position += direction * speed * dt;

    // Leg animation
    animationTimer += dt;
    legAngleOffset = sin(animationTimer * 10) * 20;

    // Drop pheromone
    pheromones.emplace_back(position, 255.0f, hasFood);

    updateBodyParts();
}

void Ant::updateBodyParts()
{
    bodyRotation = atan2(direction.y, direction.x) * 180 / 3.14159f;
    body.setPosition(position);
    body.setRotation(bodyRotation);

    sf::Vector2f headOffset = direction * 18.0f;
    head.setPosition(position + headOffset);
    head.setRotation(bodyRotation);

    sf::Vector2f perpendicular(-direction.y, direction.x);
    for (size_t i = 0; i < legs.size(); ++i)
    {
        float side = (i % 2 == 0) ? 1.0f : -1.0f;
        float positionAlongBody = (i < 3) ? -8.0f : 8.0f;
        sf::Vector2f legBase = position + direction * positionAlongBody;
        legs[i].setPosition(legBase);
        legs[i].setRotation(bodyRotation + side * (30 + legAngleOffset * (i % 2 == 0 ? 1 : -1)));
    }

    foodIndicator.setPosition(position - direction * 10.0f);
}

void Ant::draw(sf::RenderWindow &window)
{
    for (auto &leg : legs)
        window.draw(leg);
    window.draw(body);
    window.draw(head);
    if (hasFood)
        window.draw(foodIndicator);
}

void Ant::rotate(float angle)
{
    float newX = direction.x * std::cos(angle) - direction.y * std::sin(angle);
    float newY = direction.x * std::sin(angle) + direction.y * std::cos(angle);
    direction = sf::Vector2f(newX, newY);
    float len = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    direction /= len;
}
