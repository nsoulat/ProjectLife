#include "Case.hpp"

bool Case::hasAnimal() {
	if (animal && !animal->isDying()) {
		return true;
	}
	return false;
}

bool Case::hasPlant() {
	if (plant && !plant->isDying()) {
		return true;
	}
	return false;
}

bool Case::isFreeFor(Living* living) {
	switch (living->getDiet())
	{
	case Diet::Carnivorous:
	case Diet::Herbivorous:
		if (!hasAnimal()) {
			return true;
		}
		break;
	case Diet::Nothing:
		if (!hasPlant()) {
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}

bool Case::isThereAnythingToEat(Living* living) {
	Diet myDiet = living->getDiet();
	if ((hasAnimal() && animal->canBeEatenBy(myDiet)) ||
		(hasPlant() && plant->canBeEatenBy(myDiet))) {
		return true;
	}
	return false;
}

bool Case::canReproduceWithTheAnimal(Animal* living) {
	if (hasAnimal() && !animal->isBaby() && living->getDiet() == animal->getDiet()) {
		return animal->canHaveABabyWith(living->getGender());
	}
	return false;
}

void Case::moveLiving(Living* new_living) {
	switch (new_living->getDiet())
	{
	case Diet::Carnivorous:
	case Diet::Herbivorous:
		moveAnimal((Animal*)new_living);
		break;
	case Diet::Nothing:
		movePlant((Plant*)new_living);
		break;
	default:
		break;
	}
}

void Case::removeLiving(Living* living) {
	switch (living->getDiet())
	{
	case Diet::Carnivorous:
	case Diet::Herbivorous:
		animal = nullptr;
		break;
	case Diet::Nothing:
		plant = nullptr;
		break;
	default:
		break;
	}
}


void Case::moveAnimal(Animal* new_animal) {
	if (hasAnimal()) {
		animal->killed();
	}
	animal = new_animal;
}

void Case::movePlant(Plant* new_plant) {
	if (hasPlant()) {
		plant->killed();
	}
	plant = new_plant;
}

bool Case::eatPlant() {
	if (hasPlant()) {
		plant->killed();
		plant = nullptr;
		return true;
	}
	return false;
}

void Case::display(int row) {
	if (row <= 1) {
		if (hasAnimal()) {
			animal->display(row);
		}
		else {
			printf("   ");
		}
	}
	else {
		if (hasPlant()) {
			plant->display(0);
		}
		else {
			printf("   ");
		}
	}
}