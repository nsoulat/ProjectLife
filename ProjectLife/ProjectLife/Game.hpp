#pragma once

#include <iostream>
using namespace std;

#include "Case.hpp"
#include "Animal.hpp"
#include "Plant.hpp"
#include "Living.hpp"
#include <vector>

class Game {
private:
	int turn, MAX_TURN, size, speedDisplay;
	bool debug, history;
	unsigned int seed;
	Case*** board;
	vector<string> actionHistory = {};
	vector<Living*> livings;

public:
	// Constructor
	Game(const int size, const int max_turn, bool debug, int speedDisplay, bool history, unsigned int seed)
		: size{ size }, MAX_TURN{ max_turn }, debug{ debug }, speedDisplay{ speedDisplay }, seed{ seed }, history{ history }
	{
		turn = 0;
		Case*** temp = new Case**[size];
		for (int i = 0; i < size; i++) {
			temp[i] = new Case*[size];
			for (int j = 0; j < size; j++) {
				temp[i][j] = new Case(i, j, debug);
			}
		}
		board = temp;
	}

	bool isGoing() { return (getTurn() < MAX_TURN); }
	int getTurn() { return turn; }
	void incrTurn() { turn++; }


	// Board
	bool isInBoard(int x, int y);
	Case* getCase(int x, int y) { return board[x][y]; }
	bool isBoardCompleteForAnimal();
	bool isBoardCompleteForPlant();
	void getNeighborCases(int x, int y, Case* neighbors[4]);
	bool hasPlaceForReproduceOrMove(Living* living);
	bool hasFoodAround(Living* living);
	bool hasPartnerToReproduce(Animal* living);


	// Factory
	Animal* createAnimal(Diet diet, Gender gender);
	Animal* createAnimal(int x, int y, Diet diet, Gender gender);
	Plant* createPlant();
	Plant* createPlant(int x, int y);


	// History
	void addActionToHistory(string action) { actionHistory.push_back(action); }
	size_t lengthHistory() { return actionHistory.size(); }
	string getAction(int index) { return actionHistory.at(index); }
	void clearHistory() { actionHistory.clear(); }


	// Action
	void playersPlay();
	bool eat(Animal* living);
	bool reproduce(Living* living);
	void remove(Living* living);
	bool move(Animal* living, bool gonnaEat = false);
	void moveTo(Living* living, int x, int y);
	void removeDead();


	// Display
	void displayAction(int row);
	void displayBoard();
		
		
	// Destructor
	~Game() {
		if (debug) { cout << "début destruction" << endl; }
		if (board) {
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size; j++) {
					delete board[i][j];
				}
				delete[] board[i];
			}
			delete[] board;
		}
	}
};