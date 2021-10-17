### Consignes et règles du jeu

On veut simuler la croissance et décroissance de populations animales. On
considérera une zone d'une taille donnée, représentée par des cases. Chaque case peut
recevoir un animal et un végétal (mais pas 2 animaux ni 2 végétaux). Les animaux
peuvent se déplacer d'une case par tour, les végétaux restent immobiles.

Si deux animaux de la même espèce se croisent (sont sur des cases côte à côte)
un nouvel animal de la même espèce apparaîtra sur une case proche.

Tous les végétaux se développeront et feront apparaître sur toutes les cases côte
à côte avec eux un nouveau végétal tous les X tours (mettez un nombre qui permet
aux plantes de subsister).

Il existera des animaux herbivores et carnivores, les carnivores mangent les
herbivores, les herbivores mangent les plantes.

Le but est de rendre un code soigné, en C++, et de respecter les bonnes
pratiques et/ou outils que l’on a vu. Le rendu doit être un exécutable directement
fonctionnel et le code source du projet.

L’affichage peut être fait en console ou en utilisant une bibliothèque graphique
de votre choix (notamment Open Mesh).

Si vous avez du temps et souhaitez aller plus loin vous pouvez tester vos choix
d’architecture de code en essayant d’ajouter les bonus et en équilibrant les valeurs
pour que chaque espèce survive le plus possible !

Bonus :
- Ajouter un genre aux animaux (mâle et femelle)
- Après une naissance le nouveau né reste proche d’un parent pendant plusieurs tours
- Les animaux peuvent mourir de faim si ils restent trop longtemps sans manger.
- Les animaux qui viennent de se nourrir n’ont plus faim pour quelques tours
- Les animaux recherchent leur nourriture ou un partenaire de reproduction à
quelques cases autour d’eux. Ils essaient aussi de fuir pour ne pas être mangés.