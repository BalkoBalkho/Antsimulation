
#include "Ant.h"


float Ant::Mouth::pickUpFood(Food& food) {
	int food_amount = food.quantity;
	food.quantity = std::min(food.quantity - 1.0f, 0.0f);
	foodamount += food_amount;


};

bool Ant::Mouth::try_eat() {
	if (foodamount > 0) {
		foodamount-= 0.2f;
		return true;
	}
	return false;
}

void Ant::Mouth::dropFood(sf::Vector2f pos) {
	auto f = Food(pos);
	f.quantity = foodamount;
	foodamount = 0;
	f.shape.setRadius(4);

	
}