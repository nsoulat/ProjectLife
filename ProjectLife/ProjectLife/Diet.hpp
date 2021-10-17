#pragma once

#include <iostream>
using namespace std;

enum class Diet { Carnivorous, Herbivorous, Nothing };

inline string dietString(Diet const& diet) {
	string stringDiet;
	switch (diet)
	{
	case Diet::Carnivorous:
		stringDiet = "carnivorous";
		break;
	case Diet::Herbivorous:
		stringDiet = "herbivorous";
		break;
	case Diet::Nothing:
		stringDiet = "vegetal";
		break;
	default:
		stringDiet = "unknown";
		break;
	}
	return stringDiet;
}