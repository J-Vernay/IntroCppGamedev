# Cahier des charges du projet DinoRanch

Les fonctionnalités suivantes sont à faire en autonomie pour être évalué pour le rendu du projet.
Cf **evaluation.md** pour les détails des notes et les critères d'évaluation.

En cas de doute sur ce qui est attendu, se référer à l'exécutable fourni à la racine du dépôt de code.

## Partie 4 - Lasso

- Cf. TD

## Flow du jeu

- Un chronomètre de 60 secondes est affiché en haut de l'écran (centré horizontalement) et décroit.
- Plus le chronomètre est bas, plus les animaux apparaissent vite.
- En cours de partie, n'importe quel joueur peut mettre en pause avec start.
- En pause, les animaux, joueurs, et le chronomètre, ne bougent pas.
- Avant la première partie, les joueurs sont dans l'état de "lobby"
  et peuvent se connecter (start) ou se déconnecter (select).
- Un arbre de chaque saison est affiché sur le lobby ; n'importe quel joueur
  peut en entourer un pour lancer la partie avec un terrain correspondant à cette saison.
- Les joueurs ne peuvent se connecter/déconnecter que sur le lobby.
  Les joueurs ne peuvent mettre en pause que quand le jeu est en cours.
- Quand le chronomètre expire, on est de retour dans l'état "lobby".
  Les arbres sont translucides pendant 5 secondes et ne peuvent pas être sélectionnés.

## Scoring

Quand le lasso fait une boucle, le joueur gagne 10 points par animal.

Sur le côté gauche de l'écran, le score des 4 joueurs est affiché, chacun suivant la couleur du dinosaure.
Ce score reste visible pendant le temps où les joueurs sont sur le lobby.

