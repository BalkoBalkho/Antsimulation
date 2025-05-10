#pragma once
#include "Ant.h"

#include "Pheromone.h"
#include <functional>
#include <set>
#include "Simulation.h"

void Ant::Job::execute(Ant& self, float dt) {
	// Execute the job's work function
	//HashGrid<Ant> antGrid;

	eyes info = {

		siml.antGrid.getNearby(self.position),
		siml.foodGrid.getNearby(self.position),
		self.colony->pheromoneGrid.getNearby(self.position)

	};
	do_The_work(self, dt, info);
	job_progress += 0.1f; // Increment job progress
}
float Ant::Job::getNeediness(Ant& self, float dt)
{
	return neediness_of_this_job(self);
}
;

// Initialize the static vector with job definitions
// This is where you define the jobs and their behaviors
//99% ant AI is handled here. 
const std::vector<Ant::Job> Ant::available_jobs = {
	/*0*/      Job("1. Roaming around for food.", pherotype::food_finding,
			  [](Ant& t,float dt, eyes info) {
			//t.spray(pherotype::food_finding, 0.5f, t.position);
			t.moveRandomly(dt);
		   if (info.n_foods.size() > 0) { //food found
			   auto ps = info.n_foods.at(0).get()->position;

			   //t.memories.emplace("food",ps);
			   t.spray(pherotype::food, 1.0f,t.position);
				t.jobst.pop();
			   t.jobst.push(Ant::available_jobs.at(2)); //food spotted

			   return;
		   }
		   else {

		   }

	   },
	   [](Ant& t) -> float {
			// Does she have nothing better to do? might as well, ive observed they just randomly move around for no reason so high priority
			return 0.0f + (float)t.hunger / 10.0f;
		}
	),

		/*1*/  Job("2. follow food trails", pherotype::food_finding,
			[](Ant& t,float dt, eyes info) {
						if (info.n_foods.size() > 0) {
						auto ps = info.n_foods.at(0).get()->position;
						if (get_distance(ps, t.position) < 10.0f)
						t.jobst.pop();
						t.mouth.pickUpFood(*info.n_foods.at(0));
						t.jobst.push(Ant::available_jobs.at(3));
						t.spray(pherotype::food, 0.8f, t.position);
					   }
						else t.follow_pherenome_trail(pherotype::food, dt, true);
				},
				[](Ant& t) -> float {


					return 0.5f; // Example: Replace with actual logic  
				}
			),

		/*3*/  Job("3. go home with food", pherotype::food_finding,
			[](Ant& t,float dt, eyes info) {

				t.gotoCORD(t.memories.at("home"), dt);
				if (get_distance(t.memories.at("home"), t.position) < 10.0f) {
					t.jobst.pop();
					t.mouth.dropFood(t.position);
					t.spray(pherotype::food, 0.8f, t.position);
					t.jobst.pop();
				}
			},
				[](Ant& t) -> float {


					return -1.0f; // Example: Replace with actual logic  
				}
			),
		// Add more jobs as needed  
};






bool Ant::Job::JobStack::push(const Job& job) {

	if (jobNames.find(job.name) != jobNames.end()) {
		return false;
	}

	jobs.push_back(job);
	jobNames.insert(job.name);
	return true;
}

bool Ant::Job::JobStack::remove(const std::string& name) {
	auto it = std::find_if(jobs.begin(), jobs.end(),
		[&](const Job& j) { return j.name == name; });
	if (it != jobs.end()) {
		jobs.erase(it);
		jobNames.erase(name);
		return true;
	}
	return false;
}

bool Ant::Job::JobStack::contains(const std::string& name) const {
	return jobNames.find(name) != jobNames.end();
}

Ant::Job& Ant::Job::JobStack::getCurrent() {
	if (currentJobIndex >= 0 && static_cast<size_t>(currentJobIndex) < jobs.size()) {
		return jobs[currentJobIndex];
	}
	throw std::out_of_range("No current job set or index out of range.");


}

void Ant::Job::JobStack::setCurrent(const std::string& name) {
	for (size_t i = 0; i < jobs.size(); ++i) {
		if (jobs[i].name == name) {
			currentJobIndex = static_cast<int>(i);
			break;
		}
	}
}

void Ant::Job::JobStack::pop() {
	currentJobIndex = -1;
}

const std::vector<Ant::Job>& Ant::Job::JobStack::getAllJobs() const {
	return jobs;
}

size_t Ant::Job::JobStack::size() const {
	return jobs.size();
}
