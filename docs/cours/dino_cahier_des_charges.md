# Cahier des charges du projet DinoRanch

Les fonctionnalités suivantes sont à faire en autonomie pour être évalué pour le rendu du projet.
Cf **evaluation.md** pour les détails des notes et les critères d'évaluation.

En cas de doute sur ce qui est attendu, se référer à l'exécutable fourni à la racine du dépôt de code.

## Flow du jeu

- Un chronomètre de 60 secondes est affiché en haut de l'écran (centré horizontalement) et décroit. Ok
- >OK
- Plus le chronomètre est bas, plus les animaux apparaissent vite.
- >OK
- En cours de partie, n'importe quel joueur peut mettre en pause avec start.
- >OK
- En pause, les animaux, joueurs, et le chronomètre, ne bougent pas.
- >OK
- Avant la première partie, les joueurs sont dans l'état de "lobby"
  et peuvent se connecter (start) ou se déconnecter (select).
- >OK
- Un arbre de chaque saison est affiché sur le lobby ; n'importe quel joueur
  peut en entourer un pour lancer la partie avec un terrain correspondant à cette saison.
- >OK
- Les joueurs ne peuvent se connecter/déconnecter que sur le lobby.
  Les joueurs ne peuvent mettre en pause que quand le jeu est en cours.
- >OK
- Quand le chronomètre expire, on est de retour dans l'état "lobby".
  Les arbres sont translucides pendant 5 secondes et ne peuvent pas être sélectionnés.

## Scoring

Quand le lasso fait une boucle, le score donné au joueur dépend des animaux :
pour chaque type d'animaux (vache, autruche, cochon et mouton),
le premier animal de ce type rapporte 10 points, puis 20 points, puis 30 points, etc.
Par exemple, faire une boucle de lasso contenant 4 vaches et 2 autruches rapporte
**(10 + 20 + 30 + 40) + (10 + 20) = 130 points** au joueur.

Quand les animaux disparaissent, une petite notification contenant le texte +10, +20, etc,
apparaît à leur emplacement, avec le texte colorié suivant le joueur qui a fermé la boucle.

Sur le côté gauche de l'écran, le score des 4 joueurs est affiché, chacun suivant la couleur du dinosaure.
Ce score reste visible pendant le temps où les joueurs sont sur le lobby.

## Menu d'options
> Ok

Quand la partie est mis en pause, l'écran de pause continue d'afficher le jeu en arrière-plan,
et propose les choix suivants :

- Recommencer : fait réapparaître les joueurs sur le même terrain, avec le chronomètre réinitialisé.
- Retour au lobby : finit la partie, les scores ne sont plus affichés.
- Chrono : avec les flèches de gauche/droite, le chronomètre peut être avancé/réculé de 10 secondes.
- Reprendre : continue la partie, permet de sortir de la pause.

Sur l'écran de pause, tous les joueurs peuvent contrôler le menu.
L'écran de pause est contrôle avec les boutons `dpad` et `btn_right`.

