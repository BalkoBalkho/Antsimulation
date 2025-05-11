
#include "Ant.h"
#include "Simulation.h"

float Ant::Mouth::pickUpFood(Food& food) {
	float food_amount = food.quantity;
	food.quantity = std::min(food.quantity - 1.0f, 0.0f);
	foodamount += food_amount;
	this->owner->hunger -= food_amount;
	this->owner->hasFood = true;
	return food_amount;
};

bool Ant::Mouth::try_eat() {
	if (foodamount > 0) {
		foodamount-= 0.2f;
		this->owner->hunger += 0.2f;

		if (!siml.soundplayed)
		{
			siml.eatSound.stop();
			siml.eatSound.play();
			siml.soundplayed = true;
		}
		return true;
		if (foodamount <= 0) {
			foodamount = 0;
			this->owner->hasFood = false;
		}
	}
	return false;
}

void Ant::Mouth::dropFood(sf::Vector2f pos) {
	auto f = Food(pos);
	f.quantity = foodamount;
	foodamount = 0;
	f.shape.setRadius(4);

	
}