# Cahier des charges du projet DinoRanch

Les fonctionnalités suivantes sont à faire en autonomie pour être évalué pour le rendu du projet.
Cf **evaluation.md** pour les détails des notes et les critères d'évaluation.

En cas de doute sur ce qui est attendu, se référer à l'exécutable fourni à la racine du dépôt de code.

## Partie 4 - Lasso

- Cf. TD dans `dino.md`

## Flow du jeu

- Un chronomètre de 60 secondes est affiché en haut de l'écran (centré horizontalement) et décroit. fait
- Plus le chronomètre est bas, plus les animaux apparaissent vite. fait
- En cours de partie, n'importe quel joueur peut mettre en pause avec start. fait
- En pause, les animaux, joueurs, et le chronomètre, ne bougent pas. fait
- Avant la première partie, les joueurs sont dans l'état de "lobby" 
  et peuvent se connecter (start) ou se déconnecter (select). fait
- Un arbre de chaque saison est affiché sur le lobby ; n'importe quel joueur 
  peut en entourer un pour lancer la partie avec un terrain correspondant à cette saison. fait
- Les joueurs ne peuvent se connecter/déconnecter que sur le lobby. fait 
  Les joueurs ne peuvent mettre en pause que quand le jeu est en cours. fait
- Quand le chronomètre expire, on est de retour dans l'état "lobby".
  Les arbres sont translucides pendant 5 secondes et ne peuvent pas être sélectionnés. fait

## Scoring

Quand le lasso fait une boucle, le joueur gagne 10 points par animal. fait

Sur le côté gauche de l'écran, le score des 4 joueurs est affiché, chacun suivant la couleur du dinosaure.
Ce score reste visible pendant le temps où les joueurs sont sur le lobby.
