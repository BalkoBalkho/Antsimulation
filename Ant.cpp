#pragma once
#include "Ant.h"
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include "Simulation.h"
Ant::Ant(sf::Vector2f pos, Colony* colony)
    : position(pos), speed(ANT_SPEED), hasFood(false),
      legAngleOffset(0), animationTimer(0), bodyRotation(0), mouth(this)
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

    this->colony = colony;
    memories.emplace("home", pos);
}


 void Ant::spray(pherotype p, float strength, sf::Vector2f pos,bool is_request) {
	std::shared_ptr<Pheromone> pt = std::make_shared<Pheromone>(pos, p, strength);
    colony->pheromones.push_back(pt);
    
//	colony->pheromoneGrid.add(colony->pheromones.back());

}

void Ant::moveRandomly(float dt) {
	// Randomly change direction
	if (std::rand() % 100 < 5) // 5% chance to change direction
	{
		float angle = static_cast<float>(std::rand() % 60 - 30) * 3.14159f / 180.0f;
		rotate(angle);
	}
	    walk_straight(direction, dt);
}

float Ant::gotoCORD(sf::Vector2f target, float dt) {
    // Calculate direction to target
    sf::Vector2f dirToTarget = target - position;
    float distance = std::sqrt(dirToTarget.x * dirToTarget.x + dirToTarget.y * dirToTarget.y);

    if (distance > 0) {
        dirToTarget /= distance;  // Normalize
    }
	walk_straight(dirToTarget, dt); // Move towards target
    return distance;
}
// This function is used to move the ant in a straight line towards a target position.
// note: if an ant encouters an obstacle she will turn.
void Ant::walk_straight(sf::Vector2f dirToTarget,float dt) {

    // Predict next position
    sf::Vector2f predictedPosition = position + dirToTarget * speed * dt;

    // Check if predicted position is passable
    // this is a label, used by the goto,
    int tries = 0;

    TryMovingToPosition:
    if (siml.world.isPassable(predictedPosition) || tries > 6) {
        // Move normally if passable
        direction = dirToTarget;
        position = predictedPosition;
    }
    else {
        // Simple wall avoidance: bounce off the wall
        // This could be improved with proper pathfinding
        //sf::Vector2f wallNormal(0.0f, 1.0f); // Example wall normal
        //direction = direction - 2.0f * dot(direction, wallNormal) * wallNormal;

        rotate(3.14159f / 2); // Turn 90 degrees if an obstacle is detected

        predictedPosition = position + direction * speed * (dt);;
        tries++;
        goto TryMovingToPosition; //bro found a use for goto, a terrible C++ feature
    }

    // Leg animation
    animationTimer += dt;
    legAngleOffset = std::sin(animationTimer * 10) * 20;

    // Update body parts
    updateBodyParts();
    
}
void Ant::follow_pherenome_trail(pherotype pl, float dt, bool weakest = false) {
	// Find the strongest pheromone of the specified type
	Pheromone* extreme =nullptr;
    float maxStrength = 0.0f;
    float minStrenght = 999999999.9f;

	for (auto& p : colony->pheromones)
	{
        if (pq.already_visited(p.get())) continue;
		if (!weakest && p->getType() == pl && p->strength > maxStrength )
		{
			maxStrength = p->strength;
			extreme = p.get();
		}
		if (weakest && p->getType() == pl && p->strength < minStrenght)
		{
			minStrenght = p->strength;
            extreme = p.get();
			
		}

	}
	if (extreme)
	{
		pq.push(extreme);
		gotoCORD(extreme->getPosition(), dt);
    }
    else {
        moveRandomly(dt);
    }

}

const std::vector<Ant::Job>& Ant::get_all_jobs()
{
    return available_jobs;
}

const std::vector<Ant::Job>& Queen::get_all_jobs()
{
    return queen_jobs;
}



void Ant::update(float dt) {
    //	First it checks the jobs queue to see if its empty. If so she needs to find new jobs.
    //	If theres a job she checks if she should break it, maybe shes hungry or just "bored".
    //	If its fine she starts executing a bit of the job. Job will add job progress


    if (jobst.size() == 0) {
		// If no jobs, find new ones
        // the pointers are not regenerted, good.
        
        auto neeeded_jobs = available_jobs;
        
		std::sort(neeeded_jobs.begin(), neeeded_jobs.end(), [this, dt]( Job& a,  Job& b) {
			return a.getNeediness(*this, dt) > b.getNeediness(*this, dt);
			});
        std::remove_if(neeeded_jobs.begin(), neeeded_jobs.end(), [this, dt]( Job& a) {
			return a.getNeediness(*this, dt) < 0.0f;
			});
		/*std::set<Job> jobst;
		std::for_each(neeeded_jobs.begin(), neeeded_jobs.end(), [this](Job& a) {*/
			//});
        for (auto pj : neeeded_jobs) {
        
			jobst.push(pj);
        }
    }
    auto j = jobst.getCurrent();
    
    for (auto pj : available_jobs) {
		if (jobst.contains(pj.name)) {
			continue;
		}// >0.9 is for extremely important jobs that cant be ignored. extreme hunger, danger, mind control.
		if (pj.getNeediness(*this,dt) >= 0.9f) {
			jobst.push(pj);
		}
    }

    if (j.job_progress >= 1.0f) {
		jobst.pop(); //completed
	}
	else if (j.patience <= 0) {
		jobst.pop(); //no progress in time. failed
	}
	else {
		j.execute(*this, dt);
	}
    
    age++;
   

    
}
//
//void Ant::update(float dt, std::vector<Pheromone> &pheromones, const std::vector<Food> &foods, const sf::Vector2f &nestPos)
//{
//    if (std::rand() % 100 < 5)
//    {
//        float angle = static_cast<float>(std::rand() % 60 - 30) * 3.14159f / 180.0f;
//        rotate(angle);
//    }
//
//    // Follow pheromone
//    Pheromone *strongest = nullptr;
//    float maxStrength = 0.0f;
//    for (auto &p : pheromones)
//    {
//        float dist = hypot(p->getPosition().x - position.x, p->getPosition().y - position.y);
//        if (dist < 40.0f && p->hasFood != hasFood && p->strength > maxStrength)
//        {
//            maxStrength = p->strength;
//            strongest = &p;
//        }
//    }
//
//    if (strongest && maxStrength > 50.0f) { // Threshold for pheromone strength
//        sf::Vector2f dirToPheromone = strongest->getPosition() - position;
//        float len = sqrt(dirToPheromone.x * dirToPheromone.x + dirToPheromone.y * dirToPheromone.y);
//        if (len > 0)
//            direction = dirToPheromone / len;
//    } else {
//        // If no strong pheromone is found, move randomly
//        if (std::rand() % 100 < 10) { // 10% chance to change direction randomly
//            float angle = static_cast<float>(std::rand() % 360) * 3.14159f / 180.0f;
//            direction = sf::Vector2f(std::cos(angle), std::sin(angle));
//        }
//    }
//
//    // Move
//    position += direction * speed * dt;
//
//    // Leg animation
//    animationTimer += dt;
//    legAngleOffset = sin(animationTimer * 10) * 20;
//
//    // Drop pheromone
//    pheromones.emplace_back(position, 255.0f, hasFood);
//
//    updateBodyParts();
//}

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

Queen::Queen(sf::Vector2f pos, Colony* colony) : Ant(pos,colony) {

	// Initialize queen-specific properties
	speed = 0.1f; // Queen doesn't move like worker ants
	hasFood = false;
	isSelected = false;
	age = 0;
	hunger = 0;
	hp = 100; // Queen has more HP
    this->colony = colony;
    body.setOutlineThickness(10);

    
}

Colony::Colony(int number_of_ants, sf::Color color, sf::Vector2f pos  )
{
    // Initialize the colony with a given number of ants and color
    for (int i = 0; i < number_of_ants; ++i) {
        auto temp = std::make_shared<Ant>(pos, this);

        ants.push_back(temp);
        //ants.emplace_back(/*std::make_shared<Ant>*/(sf::Vector2f(0, 0), this));
        ants.back()->colony = this;
        ants.back()->body.setFillColor(color);
        ants.back()->head.setFillColor(color);
        ants.back()->foodIndicator.setFillColor(color);
    }
	auto queen = std::make_shared<Queen>(pos,this);
    queens.push_back(queen);
    queens.back()->colony = this;
    queens.back()->body.setFillColor(color);
    queens.back()->head.setFillColor(color);
    queens.back()->foodIndicator.setFillColor(color);

}

void Colony::update(float dt) {
    for (auto& p : pheromones)
            p->strength *= 1;
    /*pheromones.erase(remove_if(pheromones.begin(), pheromones.end(),
        [](const auto& p)
            { return p->strength < 0.01f; }),
                pheromones.end());*/
    
    for (auto& q : queens) q->update(dt);
    for (auto& a : ants) a->update(dt);

}

