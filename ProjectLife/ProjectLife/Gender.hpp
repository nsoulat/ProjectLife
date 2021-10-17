#pragma once

#include <iostream>
using namespace std;

enum class Gender { Male, Female, Nothing };

inline string genderString(Gender const& gender) {
	string stringGender;
	switch (gender)
	{
	case Gender::Female:
		stringGender = "female";
		break;
	case Gender::Male:
		stringGender = "male";
		break;
	case Gender::Nothing:
		stringGender = "nothing";
		break;
	default:
		stringGender = "unknown";
		break;
	}
	return stringGender;
}