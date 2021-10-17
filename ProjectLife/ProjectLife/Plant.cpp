#include <string>
#include "Plant.hpp"

int Plant::nb = 0;

int Plant::count() { return nb; }

string Plant::getInfo() {
	return dietString(getDiet()) +
		" à " + to_string(x) + " " + to_string(y) +
		", growth: " + to_string(growth) + 
		" (créée tour " + to_string(birth) + ")";
}

bool Plant::canBeEatenBy(Diet diet) {
	if (diet == Diet::Herbivorous) {
		return true;
	}
	return false;
}

bool Plant::canReproduce() {
	if (growth >= MIN_REPROCUCE){
		return true;
	}
	return false;
}