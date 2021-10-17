#pragma once

#include <iostream>
using namespace std;

#include "Living.hpp"
#include "Diet.hpp"

class Plant : public Living {
private:
	static int nb;
	string img[3] = { "§", "§", "§"};
	const int MIN_REPROCUCE = 2;

public:
	Plant(int turn, bool debug) : Living(turn, 3, 1, Diet::Nothing, debug) {
		nb++;
	}

	static int count();

	string getInfo();
	string* getImg() { return img;  }
	bool canBeEatenBy(Diet diet);
	bool canReproduce();
	void hasPlay() { grows(); }

	~Plant() {
		nb--;
		if (debug) { cout << "Plante morte" << endl; }
	};
};
