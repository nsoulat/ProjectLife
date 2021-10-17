#pragma once

#include <iostream>
using namespace std;

#include "Living.hpp"
#include "Animal.hpp"
#include "Plant.hpp"

class Case {
private:
	int x;
	int y;
	bool debug;
public:
	Animal* animal = nullptr;
	Plant* plant = nullptr;

	Case(int x, int y, bool debug) : x{ x }, y{ y }, debug{ debug } {
		if (debug) { cout << "Case créée" << endl; }
	}

	bool hasAnimal();
	bool hasPlant();

	bool isFreeFor(Living* living);
	bool isThereAnythingToEat(Living* living);
	bool canReproduceWithTheAnimal(Animal* living);

	int getX() { return x; }
	int getY() { return y; }

	void moveLiving(Living* living);
	void removeLiving(Living* living);
	void moveAnimal(Animal* new_animal);
	void movePlant(Plant* new_plant);

	bool eatPlant();

	void display(int row);

	~Case() {
		if (animal) {
			delete animal;
		}
		if (plant) {
			delete plant;
		}
		if (debug) { cout << "Case détruite" << endl; }
	}
};