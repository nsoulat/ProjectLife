#include <iostream>
using namespace std;

#include <cstring>
#include <stdlib.h>     /* strtol */
#include <string>
#include <stdio.h>

#include "Animal.hpp"
#include "Plant.hpp"
#include "Living.hpp"
#include "Game.hpp"
#include "Case.hpp"

void affiche_aide(int erreur, int argc, int* pt_plants, int* pt_herbiv, int* pt_size, int* pt_max_turn, bool* pt_debug, int* pt_speed_display, bool* pt_history, unsigned int* pt_seed);
void recup_param(int argc, char** argv, int* pt_plants, int* pt_herbiv, int* pt_size, int* pt_max_turn, bool* pt_debug, int* pt_speed_display, bool* pt_history, unsigned int* pt_seed);

int main(int argc, char** argv) {
	int nbPlants, nbHerbivorous = -1;
	int size = 7;
	int max_turn = 10;
	bool debug = false;
	bool history = true;
	unsigned int seed = (unsigned int)time(NULL); // "random" new seed
	int speed_display = 1000; // 1000 ms
	recup_param(argc, argv, &nbPlants, &nbHerbivorous, &size, &max_turn, &debug, &speed_display, &history, &seed);

	Game* game = new Game(size, max_turn, debug, speed_display, history, seed);

	srand(seed); 
	if(nbPlants == -1) { nbPlants = rand() % (size * size - size - 5 + 1) + size; }// between size and size*size - 5
	if(nbHerbivorous == -1) { nbHerbivorous = rand() % (size * size - size - 5 + 1) + size; } 

	// init
	int i;
	for (i = 0; i < nbHerbivorous; i++) {
		game->createAnimal(Diet::Herbivorous, Gender::Nothing);
	}
	for(i = 0; i < nbPlants; i++){
		game->createPlant();
	}
	if (Animal::count() >= 6) {
		game->createAnimal(Diet::Carnivorous, Gender::Female);
		game->createAnimal(Diet::Carnivorous, Gender::Male);
	}

	game->displayBoard();
	game->clearHistory();

	while (game->isGoing()) {
		game->incrTurn();
		game->playersPlay();
		game->removeDead();
	}

	delete game;

	cout << "\n\nMerci d'avoir utilisé l'écosystème !\n Auteur: Nathanaël SOULAT " << endl;

	return 0;
}

/* Fonction qui affiche l'aide */
void affiche_aide(int erreur, int argc, int* pt_plants, int* pt_herbiv, int* pt_size, int* pt_max_turn, bool* pt_debug, int* pt_speed_display, bool* pt_history, unsigned int* pt_seed)
{
	switch (erreur) {
	case 0:
		break;
	default:
		printf("\n\033[31m ERREUR: Les paramètres rentrés ne sont pas valides.\033[0m");
		break;
	}

	string isDebug = *pt_debug ? "true" : "false";
	string hasNoHistory = !*pt_history ? "true" : "false";
	string help = string("\n\n----- Aide -----\n") +
		"Ce programme permet de lancer l'écosystème.\n\n" +
		"A propos de l'écosystème:\n" +
		"Veuillez lire le README.md : https://github.com/nsoulat/ProjectLife/blob/main/README.md\n\n" +
		"Paramètres:\n" +
		"-h (--help): pour afficher ladite help\n" +
		"-s (--size) INT > 2: taille (côté) du tableau [défaut: %d]\n" +
		"-p (--plant) INT >= 0: nombre de plantes au départ [défaut: random entre size et (size*size)-1]\n" +
		"-m (--herbivorous) INT >= 0: nombre d'herbivores au départ [défaut: random entre size et (size*size)-1]\n" +
		"-t (--turn) INT >= 1: nombre de tours [défaut: %d] \n\n" +
		"Paramètres avancés:\n" +
		"--debug: pour afficher des informations avancées [défaut: %s]\n" +
		"--display INT >= 100: durée d'affichage (ms) [défaut: %dms]\n" +
		"--no-history: n'affiche pas l'historique des actions [défaut: %s]\n" +
		"--seed ULONG > 0: permet de choisir la seed pour srand() [current: %d]\n\n" +
		"Exemple: ./ProjectLife.exe --plant 50 --size 8 --debug\n" +
		"Ou encore: ./ProjectLife.exe -p 50 -s 8 -d\n\n";

	printf(help.c_str(), *pt_size, *pt_max_turn, isDebug.c_str(), *pt_speed_display, hasNoHistory.c_str(), *pt_seed);
}

/* Fonction qui récupère les paramètres de la ligne de commande et les stock  */
void recup_param(int argc, char** argv, int* pt_plants, int* pt_herbiv, int* pt_size, int* pt_max_turn, bool* pt_debug, int* pt_speed_display, bool* pt_history, unsigned int* pt_seed)
{
	auto returnError = [&](int error)
	{
		affiche_aide(error, argc, pt_plants, pt_herbiv, pt_size, pt_max_turn, pt_debug, pt_speed_display, pt_history, pt_seed);
		exit(1);
	};
	char* pEnd;
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
			returnError(0);
		}
		else if (!strcmp(argv[i], "--debug")) {
			*pt_debug = true;
		}
		else if (!strcmp(argv[i], "-p") || !strcmp(argv[i], "--plant")) {
			if (i == argc - 1) { returnError(2); }
			*pt_plants = stoi(argv[++i]);
			if (*pt_plants < 0) { returnError(2); }
		}
		else if (!strcmp(argv[i], "-m") || !strcmp(argv[i], "--herbivirous")) {
			if (i == argc - 1) { returnError(2); }
			*pt_herbiv = stoi(argv[++i]);
			if (*pt_herbiv < 0) { returnError(2); }
		}
		else if (!strcmp(argv[i], "-t") || !strcmp(argv[i], "--turn")) {
			if (i == argc - 1) { returnError(2); }
			*pt_max_turn = stoi(argv[++i]);
			if (*pt_max_turn < 1) { returnError(2); }
		}
		else if (!strcmp(argv[i], "-s") || !strcmp(argv[i], "--size")) {
			if (i == argc - 1) { returnError(2); }
			*pt_size = stoi(argv[++i]);
			if (*pt_size < 3) { returnError(2); }
		}
		else if (!strcmp(argv[i], "--display")) {
			if (i == argc - 1) { returnError(2); }
			*pt_speed_display = stoi(argv[++i]);
			if (*pt_speed_display < 100) { returnError(2); }
		}
		else if (!strcmp(argv[i], "--no-history")) {
			*pt_history = false;
		}
		else if (!strcmp(argv[i], "--seed")) {
			if (i == argc - 1) { returnError(2); }
			*pt_seed = strtoul(argv[++i], &pEnd, 0);
			if (*pt_seed <= 0) { returnError(2); }
		}
		else {
			returnError(2);
		}
	}
}
