#pragma once
#include <vector>
#include "Ant.h"
#include "Pheromone.h"
#include "Simulation.h"
class MindControl {
	Ant::Job::JobStack jcopy;
	sf::Vector2f goHere;
	std::weak_ptr<Ant> controlled_ant;
	//Ant::Job mindControlJob;
	Pheromone sp;

public:	
MindControl()= default;
static enum class inputType{
	CONTROL,
	GO_HERE,
	SPRAY,
};
void controlAnt(std::shared_ptr<Ant> victm) {

	if (!controlled_ant.expired())	{ 
		controlled_ant.lock()->jobst.pop();
	}
		
			victm->jobst.push(Ant::Job("mind control", pherotype::transport,
			[](Ant& t, float dt, Ant::eyes info) {
				
				t.gotoCORD(t.memories.at("Mind Control Go Here"), dt);
				

			},
			[](Ant& t) -> float {


				return 1.0f; 
			}
		));
		controlled_ant = victm;
		
	
}

void update() {
	if (!controlled_ant.expired()) {
		controlled_ant.lock()->memories["Mind Control Go Here"] = goHere;
	}
}

void processInputs(std::vector<std::pair<sf::Vector2f,inputType>> v) {

	for (auto input : v) {
		auto& position = input.first;
		auto& type = input.second;
		switch (type)
		{ 
			case inputType::CONTROL:
				for (auto& a : siml.all_ants()) {
					if (get_distance(a->getPosition(), position) < 5.0f) {
						controlAnt(a);
					}
				
				}
				break;
			case inputType::GO_HERE:
				goHere = position;
				break;
			case inputType::SPRAY:
			break;
			
		}

	}
}



};