// Antsimulation.h : Include file for standard system include files,
// or project specific include files.

#pragma once


#include "Pherenome.h"
#include <iostream>
#include "World.h"


class Simulation {

PherenomeFactory pF;
vector <Colony> colonies;
// gui gui


public: 
World world;
float timestep;
void init();

}

// globals that all will access from "s::w"
Simulation s;



// TODO: Reference additional headers your program requires here.
