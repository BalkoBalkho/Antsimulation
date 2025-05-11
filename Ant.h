#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Pheromone.h"
#include "Constants.h"
#include "Food.h" 
#include <queue>
#include <unordered_set>
#include <map>
#include "PherenomeQueue.hpp"
#include "hashgrid.hpp"


//class MindControl;
//class Colony;

class Ant {
    private:
	friend class MindControl;
	friend class Colony;
	friend class Queen;
	PherenomeQueue pq;
	struct eyes {
		std::vector<std::shared_ptr<Ant>> n_ants;
		std::vector<std::shared_ptr<Food>> n_foods;
		std::vector<std::shared_ptr<Pheromone>> n_pheromones;

	};
	class Mouth {
			enum class MouthState {
				baby,
				food,
				empty,
				dirt
			};
			uint32_t foodamount = 0;
			Ant* owner;
		public:
			//void eat(Food& food);
			float pickUpFood(Food& food);
			bool try_eat();
			void dropFood(sf::Vector2f pos);
			Mouth(Ant* owner) : owner(owner) {};
			//void pickUpDirt();
			//void pickUpPupae();
			//void dropPupae();

		};
		Mouth mouth;
	class Job {
		using lambda = void(*)(Ant&, float dt,eyes info);
		using get_percentage = float(*)(Ant&);
		lambda do_The_work;
		get_percentage neediness_of_this_job;
	public:
		std::string name =""; // Name of the job for debugging or identification
		pherotype type = pherotype::colony;
		float job_progress = 0.0f; // How far along the ant is in completing the job,
		int patience =100; // How long the ant will wait before abandoning the job

		void execute(Ant& self, float dt);
		float getNeediness(Ant& self, float dt);

		Job(std::string name, pherotype type, lambda do_The_work, get_percentage neediness_of_this_job)
			: type(type), do_The_work(do_The_work), neediness_of_this_job(neediness_of_this_job), name(name) {
		}

		// Static vector to store all jobs
		const static std::vector<Job> jobs;

		class JobStack {
		protected:
			std::vector<Job> jobs;
			std::unordered_set<std::string> jobNames;
			int currentJobIndex = -1;

		public:
			bool push(const Job& job);
			bool remove(const std::string& name);
			bool contains(const std::string& name) const;
			Job& getCurrent();
			void setCurrent(const std::string& name);
			void pop();
			const std::vector<Job>& getAllJobs() const;
			size_t size() const;
		};

	};
		
		Colony* colony; // Pointer to the colony the ant belongs to. This pointers lifetime is long, no need to manage.	
        const static std::vector<Job> available_jobs;
		float favorite_number = 0.3f; // A random number for the ant's behavior, not used yet.
		static constexpr bool is_movable = true;// can this entity move? internally used in hashgrid.


        //rendering
        float speed;  //Determines how fast the ant moves in the simulation.
        float legAngleOffset;  // Controls the offset for animating the ant's leg movement.
        float animationTimer;  // Tracks time for animating the ant's legs or other body parts.
        float bodyRotation;  // Stores the current rotation angle of the ant's body for directional movement.

        sf::ConvexShape body;//Represents the main body of the ant. A convex shape allows for a custom polygonal shape to visually depict the ant's body.
        sf::CircleShape head;// Represents the head of the ant as a circle.
        sf::CircleShape foodIndicator;//A small circle used to indicate whether the ant is carrying food (e.g., drawn when hasFood is true).
        std::vector<sf::RectangleShape> legs;// A collection of rectangle shapes representing the ant's legs, which can be animated for movement.

        void updateBodyParts();

		Job::JobStack jobst; // Stack to manage jobs for the ant
        int age;
        int hunger;
		int hp;
		std::map<std::string, sf::Vector2f> memories; //used by job system, colony location and such. 
    public:
        sf::Vector2f position; // Tracks the ant's location in the simulation.
        sf::Vector2f direction; //Tracks the direction the ant is moving.
        bool hasFood;  //Tracks whether the ant is carrying food.
        bool isSelected;  //Tracks whether the ant is selected by the user for interaction or tracking.
		Ant() = default;
		Ant(sf::Vector2f pos, Colony* colony);  //declaration of the constructor

		void spray(pherotype p, float strength, sf::Vector2f pos,bool is_request=false);

		void moveRandomly(float dt);

		float gotoCORD(sf::Vector2f pos, float dt);

		void walk_straight(sf::Vector2f dirToTarget, float dt);

		void follow_pherenome_trail(pherotype pl, float dt, bool weakest);
		const std::vector<Ant::Job>& get_all_jobs();


        //void update(float dt, std::vector<Pheromone>& pheromones, const std::vector<Food>& foods, const sf::Vector2f& nestPos);//handles ants movement, interactions with food, pheromones, and the nest.
        void update(float dt);
        void draw(sf::RenderWindow& window);   //renders ant on the screen
        void rotate(float angle);    //changes ants direction by a specified angle
};
class Queen : public Ant{
protected:
	static std::vector<Job> queen_jobs;
public:
    using Ant::Ant;
	const std::vector<Ant::Job>& get_all_jobs();
	Queen(sf::Vector2f pos, Colony* colony);
	Queen() = default;
};

// class Pupae : public Ant {
// protected:

// public:
// 	using Ant::Ant;
// 	void draw(sf::RenderWindow& window) ;
// 	Pupae();
// };

class Colony {
	
	public:
	sf::Color colony_color; // The color of the colony, used for rendering and identification.
	std::vector <std::shared_ptr<Ant>> ants; // A vector to store all the ants in the colony.
    std::vector <std::shared_ptr<Queen>> queens; // A vector to store all the queens in the colony.
	std::list<std::shared_ptr<Pheromone>> pheromones; // A list of pointers to store pheromones in the colony.
	HashGrid<Pheromone> pheromoneGrid; // A hash grid to manage (local) pheromones in the colony's area.
	void update(float dt);
	Colony(int number_of_ants = NUM_ANTS, sf::Color color = sf::Color::Red, sf::Vector2f pos = {810.0f,560.0f} ); // Constructor to initialize the colony with a specified number of ants and color.
};

