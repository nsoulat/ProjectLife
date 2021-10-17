#pragma once

#include <iostream>
using namespace std;

#include "Diet.hpp"

class Living {
private:
	static int nb;
	bool dead = false;
	Diet diet;

protected:
	int birth = 0;
	int img_width;
	int img_height;
	int growth = 0;
	bool debug;

public:
	int x = -1;
	int y = -1;

	Living(int turn, int width, int height, Diet diet, bool debug)
		: birth{ turn }, img_width{ width }, img_height{ height }, diet{ diet }, debug{ debug } {
		nb++;
	}

	void updatePosition(int x, int y) {
		this->x = x;
		this->y = y;
	}

	static int count();

	bool isDying() { return dead; }
	void killed() { dead = true; }
	Diet getDiet() { return diet; }
	void grows() { growth++; }

	virtual string getInfo() = 0;
	virtual string* getImg() = 0;
	virtual void display(int row);
	virtual bool canBeEatenBy(Diet diet) = 0;
	virtual bool canReproduce() = 0;
	virtual void hasPlay() = 0;

	string ToString() { return getInfo(); }

	virtual ~Living() {
		nb--;
	};
};

