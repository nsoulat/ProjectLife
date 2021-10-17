#include <iostream>
using namespace std;

#include <chrono>
#include <thread>
#include <algorithm>    // std::copy

#include "Game.hpp"
#include <string>

// Board
bool Game::isInBoard(int x, int y) {
	return (x >= 0 && x < size && y >= 0 && y < size);
}

bool Game::isBoardCompleteForAnimal() {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (!board[i][j]->hasAnimal()) {
				return false;
			}
		}
	}
	return true;
}

bool Game::isBoardCompleteForPlant() {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (!board[i][j]->hasPlant()) {
				return false;
			}
		}
	}
	return true;
}

// the boxes will be given in the trigonometric order but the first neighbor is random
void Game::getNeighborCases(int x, int y, Case* neighbors[4]) {

	int dx[4] = { 1, 0, -1, 0 };
	int dy[4] = { 0, 1, 0, -1 };

	int begin = rand() % 4;
	for (int i = begin; i < begin+4; i++) {
		int neighbor_x = x + dx[i%4];
		int neighbor_y = y + dy[i%4];
		if (isInBoard(neighbor_x, neighbor_y)) {
			neighbors[i%4] = getCase(neighbor_x, neighbor_y);
		}
		else {
			neighbors[i%4] = nullptr;
		}
	}
}

bool Game::hasPlaceForReproduceOrMove(Living* living) {
	Case* neighbors[4];
	getNeighborCases(living->x, living->y, neighbors);
	for (int i = 0; i < 4; i++) {
		if (neighbors[i] && neighbors[i]->isFreeFor(living))
		{
			return true;
		}
	}
	return false;
}

bool Game::hasFoodAround(Living* living) {
	Case* neighbors[4];
	getNeighborCases(living->x, living->y, neighbors);
	for (int i = 0; i < 4; i++) {
		if (neighbors[i] && neighbors[i]->isThereAnythingToEat(living))
		{
			return true;
		}
	}
	return false;
}

bool Game::hasPartnerToReproduce(Animal* living) {
	Case* neighbors[4];
	getNeighborCases(living->x, living->y, neighbors);
	for (int i = 0; i < 4; i++) {
		if (neighbors[i] && neighbors[i]->canReproduceWithTheAnimal(living))
		{
			return true;
		}
	}
	return false;
}


// Factory
Animal* Game::createAnimal(Diet diet, Gender gender) {
	if (!isBoardCompleteForAnimal()) {
		int x = rand() % size; // random int between 0 and size-1
		int y = rand() % size;
		while (board[x][y]->hasAnimal()) {
			x++;
			if (x >= size) {
				x = 0;
				y = (y >= size - 1) ? 0 : y + 1;
			}
		}
		return createAnimal(x, y, diet, gender);
	}
	addActionToHistory("Impossible de créer une nouvelle plante");
	return nullptr;
}

Animal* Game::createAnimal(int x, int y, Diet diet, Gender gender) {
	if (gender == Gender::Nothing) {
		int genderInt = rand() % 2;
		gender = genderInt == 0 ? Gender::Female : Gender::Male;
	}
	Animal* animal = new Animal(getTurn(), diet, gender, debug);
	livings.push_back(animal);
	moveTo(animal, x, y);
	string action = "Animal ajouté à " + to_string(x) + ", " + to_string(y);
	addActionToHistory(action);
	return animal;
}

Plant* Game::createPlant() {
	if (!isBoardCompleteForPlant()) {
		int x = rand() % size; // random int between 0 and size-1
		int y = rand() % size;
		while (board[x][y]->hasPlant()) {
			x++;
			if (x >= size) {
				x = 0;
				y = (y >= size-1) ? 0 : y+1;
			}
		}
		return createPlant(x, y);
	}
	addActionToHistory("Impossible de créer une nouvelle plante");
	return nullptr;
}

Plant* Game::createPlant(int x, int y) {
	string action = "Plante ajoutée à " + to_string(x) + ", " + to_string(y);
	addActionToHistory(action);
	Plant* plant = new Plant(getTurn(), debug);
	livings.push_back(plant);
	moveTo(plant, x, y);
	return plant;
}


// Action
void Game::playersPlay() {
	vector<Living*> duplicateLivings;
	duplicateLivings.assign(livings.begin(), livings.end());
	vector<Living*>::iterator it = duplicateLivings.begin();
	for (; it != duplicateLivings.end(); ) {
		Living* living = *it;
		string actionHistory = "Tour de " + living->ToString();
		addActionToHistory(actionHistory);
		displayBoard();
		if (!living->isDying()) {
			bool hasReproduced, hasEaten, hasMoved = false;
			switch (living->getDiet())
			{
			case Diet::Carnivorous:
				hasReproduced = reproduce(living);
				hasMoved = move((Animal*)living, true); // to eat it has to move on a herbivorous animal
				if (hasMoved) {
					if (!hasReproduced) { reproduce(living); }
				}
				if (((Animal*)living)->isStarving()) {
					addActionToHistory("L'animal est mort de faim...");
					remove(living);
					living->killed();
				}
				break;
			case Diet::Herbivorous:
				hasEaten = eat((Animal*)living);
				hasReproduced = reproduce(living);
				hasMoved = move((Animal*)living);
				if (hasMoved) {
					if (!hasEaten) { eat((Animal*)living); }
					if (!hasReproduced) { reproduce(living); }
				}
				if (((Animal*)living)->isStarving()) {
					addActionToHistory("L'animal est mort de faim...");
					remove(living);
					living->killed();
				}
				break;
			case Diet::Nothing:
				reproduce(living);
				break;
			default:
				break;
			}
			living->hasPlay();
		}
		++it;
		clearHistory();
	}
}

bool Game::eat(Animal* living) {
	if (living->getDiet() == Diet::Herbivorous) {
		Case* box = getCase(living->x, living->y);
		if (box->eatPlant()) {
			living->hasEaten();
			addActionToHistory("L'animal a mangé l'herbe !");
			displayBoard();
			return true;
		}
	}
	return false;
}

bool Game::reproduce(Living* living) {
	if (living->canReproduce() && hasPlaceForReproduceOrMove(living)) {
		Case* neighbors[4];
		getNeighborCases(living->x, living->y, neighbors);
		bool hasSpread = false;
		switch (living->getDiet())
		{
		case Diet::Carnivorous:
		case Diet::Herbivorous:
			if (hasPartnerToReproduce((Animal*)living)) {
				for (int i = 0; i < 4; i++) {
					if (neighbors[i] && !neighbors[i]->hasAnimal()) {
						createAnimal(neighbors[i]->getX(), neighbors[i]->getY(), living->getDiet(), Gender::Nothing);
						addActionToHistory("Un bébé est né");
						displayBoard();
						return true;
					}
				}
			}
			break;
		case Diet::Nothing:
			for (int i = 0; i < 4; i++) {
				if (neighbors[i] && !neighbors[i]->hasPlant()) {
					createPlant(neighbors[i]->getX(), neighbors[i]->getY());
					addActionToHistory("Une nouvelle plante est apparue");
					displayBoard();
					hasSpread = true;
				}
			}
			return hasSpread;
		default:
			break;
		}
	}
	return false;
}

bool Game::move(Animal* living, bool gonnaEat) {
	bool canEatNear = gonnaEat && living->canEat() && hasFoodAround(living);
	Case* neighbors[4];
	getNeighborCases(living->x, living->y, neighbors);
	for (int i = 0; i < 4; i++) {
		if (neighbors[i]) {
			if ((canEatNear && neighbors[i]->isThereAnythingToEat(living)) ||
				(!canEatNear && neighbors[i]->isFreeFor(living))) {
				int x = neighbors[i]->getX();
				int y = neighbors[i]->getY();
				moveTo(living, x, y);
				string actionHistory = "Déplacement vers " + to_string(x) + " " + to_string(y);
				addActionToHistory(actionHistory);
				if (canEatNear) {
					living->hasEaten();
					addActionToHistory("et a mangé !");
				}
				displayBoard();
				return true;
			}
		}
	}
	return false;
}

void Game::remove(Living* living) {
	int oldX = living->x;
	int oldY = living->y;
	if (isInBoard(oldX, oldY)) {
		Case* oldBox = getCase(oldX, oldY);
		oldBox->removeLiving(living);
	}
}

void Game::moveTo(Living* living, int x, int y) {
	remove(living);
	Case* box = getCase(x, y);
	box->moveLiving(living);
	living->updatePosition(x, y);
}

void Game::removeDead() {
	vector<Living*>::iterator it = livings.begin();
	for (; it != livings.end(); ) {
		Living* living = *it;
		if (living->isDying()) {
			it = livings.erase(it);
			delete living;
		}
		else {
			++it;
		}
	}
}

// Display
void Game::displayAction(int row) {
	if ((unsigned int)row < lengthHistory()) {
		cout << getAction(row);
	}
}

void Game::displayBoard() {
	int i, j, row;
	if (!debug) { system("cls||clear"); }

	string strSeed = debug ? string(" (seed : ") + to_string(seed) + ")" : "";
	printf("Ecosystème, tour: %d/%d%s\n", turn, MAX_TURN, strSeed.c_str());
	printf(" "); 
	for (j = 0; j < 5*size; j++) /* ligne horizontale */
	{
		printf("_");
	}
	printf("\n");
	for (i = 0; i < size; i++)
	{
		for (row = 0; row < 4; row++) {
			if (row < 3) {
				for (j = 0; j < size; j++)
				{
					printf(" |");
					Case* box = getCase(i, j);
					box->display(row);
				}
			}
			else {
				printf(" ");
				for (j = 0; j < 5 * size - 1; j++) {
					printf("_");
				}
			}
			if (history) {
				printf("|      ");
				displayAction(4 * i + row);
			}
			printf("\n");
		}
	}

	cout << "(Au début du tour) nb vivants : " << Living::count() << ", nb plantes : " << Plant::count() << ", nb animaux : " << Animal::count() << endl;

	this_thread::sleep_for(std::chrono::milliseconds(speedDisplay));
}
