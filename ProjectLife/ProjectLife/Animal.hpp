#pragma once

#include <iostream>
using namespace std;

#include <stdio.h>
#include "Living.hpp"
#include "Diet.hpp"
#include "Gender.hpp"

class Animal : public Living {
private:
	static int nb;
	Gender gender;
	string img[6] = { "O", "-", " ", " ", "h", "h" };
	int hunger = 0;
	const int MAX_HUNGER = 4;
	const int MIN_AGE = 2;

public:
	Animal(int turn, Diet diet, Gender gender, bool debug)
		: Living(turn, 3, 2, diet, debug), gender{ gender } {
		if (gender == Gender::Female) { img[3] = "F"; }
		if (gender == Gender::Male) { img[3] = "M"; }
		if (diet == Diet::Herbivorous) { img[1] = "~"; }
		nb++;
	}

	static int count();

	string getInfo();
	string* getImg() { return img; }
	Gender getGender() { return gender; }
	bool isBaby() { return growth <= MIN_AGE; }
	bool isStarving() { return hunger > MAX_HUNGER; }

	bool canBeEatenBy(Diet diet);
	bool canHaveABabyWith(Gender gender);
	bool canEat() { return hunger >= 1; }
	bool canReproduce();
	void hasEaten() { hunger = 0; }
	void hasPlay();
	void update();

	~Animal() {
		nb--;
		if (debug) { cout << "Animal mort" << endl; }
	};
};

