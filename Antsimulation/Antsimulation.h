// Antsimulation.h : Include file for standard system include files,
// or project specific include files.

#pragma once


#include "Pherenome.h"
#include <iostream>
#include "World.h"
#include <vector>
#include "Ant.h"

//#include "A.h" // Add this include to resolve the Colony type

class Simulation {

   PherenomeFactory pF{1}; // Use curly braces for initialization
   std::vector<Colony> colonies;

public:
   World world;
   float timestep;
   void init();
   Simulation() {};
};

// globals that all will access from "s::w"
Simulation s;




// TODO: Reference additional headers your program requires here.
