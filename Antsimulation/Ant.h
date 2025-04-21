#pragma once
#include "Pherenome.h"
#include "World.h"
#include <stack>
#include <array>


struct AntCord {
	cords head;
	cords body;
	cords stinger;

};

//Forward declaration, people hate on JS but you dont have to deal with tihs
class Ant;

class Job {
	Pherenome::jobs job_id;

public:
	virtual void Job_function(Ant job_worker) = 0;
	virtual float job_needness() = 0;
	float progress = 0; //0-1


};


class Ant {
	// This is fine since no one other class than Ant/routine accesses Job* (but the program does)
	// techincally violates encapsulation but not the spirit of it?
	// The reason Job isnt a private class in Ant is to make organisation much easier
	friend Job;
	std::stack<Job> job_stack;



	AntCord POS;

	//namespace life {
	int health;
	int age;
	bool at_home = true;
	cords homewhere;


	int sleepiness;
	int favorite_number;
	int steps_counter;



	//}
	enum class direction { UP, DOWN, LEFT, RIGHT };

	void move(direction);
	std::array<std::array<World::WorldPixel, 5>, 5> sense();

	void findJob();

public:
	void tick(); // Run every game loop.


};

//class Job {
//	Pherenome::jobs job_id;
//
//public: 
//	virtual void Job_function(Ant job_worker) =0;
//	virtual float job_needness() =0;
//	float progress=0; //0-1
//
//
//};
//
//
//

class Queen : public Ant {


};

class Colony {
	int id;

};