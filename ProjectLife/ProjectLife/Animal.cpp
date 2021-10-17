#include <string>
#include "Animal.hpp"

int Animal::nb = 0;

int Animal::count() { return nb; }

string Animal::getInfo() { 
	string baby = isBaby() ? " (baby)" : "";
	return dietString(getDiet()) +
		baby +
		" à " + to_string(x) + " " + to_string(y) +
		", age: " + to_string(growth) +
		", faim: " + to_string(hunger) +
		", " + genderString(getGender()) +
		" (créé tour " + to_string(birth) + ")";
}

bool Animal::canBeEatenBy(Diet diet) {
	switch (getDiet())
	{
	case Diet::Carnivorous:
		break;
	case Diet::Herbivorous:
		if (diet == Diet::Carnivorous) {
			return true;
		}
	default:
		break;
	}
	return false;
}

bool Animal::canHaveABabyWith(Gender gender)
{
	switch (getGender()) {
	case Gender::Male:
		if (gender == Gender::Female) {
			return true;
		}
		break;
	case Gender::Female:
		if (gender == Gender::Male) {
			return true;
		}
	default:
		break;
	}
	return false;
}


bool Animal::canReproduce() {
	if (!isBaby() && getGender() == Gender::Female){
		return true;
	}
	return false;
}

void Animal::hasPlay() {
	grows();
	hunger++;
	update();
}

void Animal::update() {
	if (growth > MIN_AGE){ 
		if (getDiet() == Diet::Carnivorous) {
			img[2] = "-";
		}
		else if (getDiet() == Diet::Herbivorous) {
			img[2] = "~";
		}
	}
	else {
		img[2] = " ";
	}
}
