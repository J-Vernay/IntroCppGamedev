# Exercice 1

**OBJECTIF**  
Se familiariser avec :
- L'organisation de la base de code
- La syntaxe du C++
- Le mini-moteur de jeu "JV" qui sera utilisé pendant tout le cours
- Les quelques mathématiques nécessaires pour suivre le cours

Dans l'explorateur de solutions, à droite, clic-droit sur "Exo 1",
puis clic sur "Définir en tant que projet de démarrage".

## TD

Vous devez compléter ce fichier au fur et à mesure du cours.

A) Résumez en une phrase le rôle des fichiers suivants :

> `jv.h` : 
> Declare un grand nombre de fonction utilitaire qui pourront etre appeler n'importe ou, et qui servent de "moteur" de jeu, API
> `jv_win64_impl.h` :
> c'est l'API interne pour les fichiers dimplementation quils puissent se parler entre eux
> `jv_win64_main.cpp` : 
> La classe principal qui va gerer et appeler les fonctions, le centre de commande du moteur
> `jv_win64_window.cpp` : 
> fonction qui creer la fenetre de jeu
> `jv_win64_renderer.cpp` :
> fonctions qui gere tout le visuel et les graphics du moteur de jeu
> `jv_win64_file.cpp` : 
> fonctions permettant dimporter des dossier dans le moteur
> `exo1_pong.cpp` : 
> fait un jeu en utilisant le moteur de jeu

B) Remettez les commentaires ci-dessous aux bons endroits,
dans les fichiers `jv_win64_main.cpp` et `jv_win64_window.cpp`:

> // Appel au système d'exploitation pour créer la fenêtre de jeu.
>
> // Déclenche l'affichage d'une frame.
>
> // Finalisation de la logique de jeu.
>
> // Variable globale.
>
> // Fonction qui implémente de la logique de création de la fenêtre de jeu.
>
> // Fonction d'entrée du programme, contient le code
> // qui sera appelé par le système d'exploitation Windows
> // quand le programme est lancé.
>
> // Initialisation du moteur de jeu.
>
> // Fonction appelée par le système d'exploitation pour transmettre les événements liés à la fenêtre.
>
> // L'utilisateur redimensionne la fenêtre, il faut transmettre l'information au rendu.
>
> // On mesure le temps écoulé.
>
> // Appel au système d'exploitation pour récupérer tous les événements émis.
>
> // Fonction qui implémente de la récupération des entrées : manettes, clavier, souris.
>
> // Fonction appelée en cas de bug pour interrompre le programme et afficher un message d'erreur.
>
> // Initialisation de la logique de jeu.
>
> // Boucle principale d'événements, qui traite les messages que le système
> // d'exploitation nous envoit. Tourne en boucle tant que le programme continue.
>
> // Met à jour la logique de jeu.
>
> // Initialisation de la mesure du temps.
>
> // Finalisation du moteur de jeu.

C) Quelles sont les 4 fonctions qu'un jeu doit fournir au moteur de jeu JV ?

> Init pour initialiser, update pour mettre a jours et Draw pour les graphiques, et shut pour lorsque le jeu se ferme

D) Dans la fonction DrawRect(), que représente 'pos' et 'size' ?

> c'est la ou tu positionne tout les vertex de tes rectangles et size et la distance entre les differents vertex

E) Qu'est-ce qu'un Vertex ? Que fait `jv::gpu::CreateVertexBuffer()` ?
Que fait `jv::gpu::Draw()` ?

> ce sont des triangles qui forme des formes geometrique, et draw une demande de rendu de triangles texturés à la carte graphique.

F) La balle se dirige vers la droite, à une vitesse de 300 pixels par seconde.
Le temps entre deux frames est 20 millisecondes. Quelle distance en pixel a été parcourue entre ces deux frames ?

> 6 pixels car on fait 300/1000*20 = 6

G) Le temps entre deux frames est 10 millisecondes. Pendant ce temps, la balle s'est dirigée
suivant le vecteur (-30, 40) (en pixels). Dans quelle direction s'est-elle déplacée ?
À quelle vitesse, en pixels par seconde, cela correspond-il ?

> la balle se dirige vers le bas a gauche, et  avec pythagore la distance correspond a 50, donc il se deplace a 5000  pixel par secondes

H) La balle est à la position (100, 200). Elle se dirige en diagonale droite-haut,
à la vitesse de 100 pixels par seconde. À quelle position la balle est-elle au bout d'une seconde ?

>  avec pythagore on peut determiner quon cest deplacer  de 71 pixel, la coordonner sera (171, 179)
I) Modifier le code pour que la balle se déplace, dépendamment de sa vitesse, de sa direction, et du temps écoulé.

J) D'après l'image ci-dessous, exprimez les coordonnées suivantes suivant les variables du code C++ :

![exo1_coords.png](exo1_coords.png)

> `x1 = playerSize.x`
>
> `x2 = ballPos.x`
>
> `x3 = ballPos.x + ballSize.X
> 
> `x4 = ArenaSize.x- AiSize.x
>
> `y1 = playerPos.y`
>
> `y2 = playerPos.y+ playerSize.y`
>
> `y3 = aiPos.y`
>
> `y4 = ballPos.y`
>
> `y5 = BallPos.y +basllSIze.y`
>
> `y6 = aiPos.y + aiSize.y


K) Implémenter les conditions de détections de collisions.

L) Implémenter les conditions de victoire du joueur et de l'IA.

**Prochain exercice : exo2.md**
