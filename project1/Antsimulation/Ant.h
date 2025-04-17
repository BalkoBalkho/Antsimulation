#pragma once
#include "Pherenome.h"

struct AntCord {
	struct head { int x; int y; };
	struct body { int x; int y; };
	struct stinger { int x; int y; };
	
};

class Ant {
	AntCord POS;


	//namespace life {
	int health; 
	int sleepiness;
	int age;
	int favorite_number;	


	

	//}
	
public :
	void tick(); // Run every game loop, potentially multithreaded.
	
};

class Queen : public Ant {


};