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

> `jv.h` : Déclare les fonctions du moteur jv.
>
> `jv_win64_impl.h` : Déclare les fonctions qui permettrons d'initialiser le moteur.
> 
> `jv_win64_main.cpp` : Appelle les fonctions définies dans les autres fichiers jv_win64, il s'occupe de gérer la boucle d'update, de mettre a jour l'affichage et de récupérer les inputs.
>
> `jv_win64_window.cpp` : Implémente les fonctions qui s'occupent de la fenêtre de jeu.
>
> `jv_win64_renderer.cpp` : Implémente les fonctions qui s'occupent du rendu graphique, de ce qui sera affiché sur la fenêtre de jeu.
>
> `jv_win64_file.cpp` : Permet de charger des fichiers venant du dossier "/Assets"
>
> `exo1_pong.cpp` : S'occupe de tout la logique du jeu, des mouvements de la balles et des barres.

B) Remettez les commentaires ci-dessous aux bons endroits,
dans les fichiers `jv_win64_main.cpp` et `jv_win64_window.cpp`:

> // 
>
> // 
>
> // 
>
> // 
>
> // 
>
> // 
>
> // 
>
> // 
>
> // 
>
> // 
>
> // 
>
> // 
>
> // 
>
> // 
>
> // 
>
> //
>
> // 
>
> // 

C) Quelles sont les 4 fonctions qu'un jeu doit fournir au moteur de jeu JV ?

> Init, Update, Draw et Shut.

D) Dans la fonction DrawRect(), que représente 'pos' et 'size' ?

> pos définit la position du sommet inférieur gauche du Rect qu'on veut dessiner, et size est sa taille

E) Qu'est-ce qu'un Vertex ? Que fait `jv::gpu::CreateVertexBuffer()` ?
Que fait `jv::gpu::Draw()` ?

> Un Vertex est le sommet d'une face dans un espace, définit par une position.
VertexBuffer prend une liste de vertex, et retourne un format de donnée fait pour le rendu.
Draw prend ce format et l'affiche a l'écran

F) La balle se dirige vers la droite, à une vitesse de 300 pixels par seconde.
Le temps entre deux frames est 20 millisecondes. Quelle distance en pixel a été parcourue entre ces deux frames ?

> 300 * 0.02 = 6, 6 pixels ont étés parcourus entre deux frames

G) Le temps entre deux frames est 10 millisecondes. Pendant ce temps, la balle s'est dirigée
suivant le vecteur (-30, 40) (en pixels). Dans quelle direction s'est-elle déplacée ?
À quelle vitesse, en pixels par seconde, cela correspond-il ?

> La balle s'est déplacée en haut a gauche. sqrt(30² + 40²) = 50. Elle s'est déplacée de 50 pixels en
1 frame, 50/0.01 = 5000, elle se déplace a 5000 pixels/secondes.

H) La balle est à la position (100, 200). Elle se dirige en diagonale droite-haut,
à la vitesse de 100 pixels par seconde. À quelle position la balle est-elle au bout d'une seconde ?

> (271, 129)

I) Modifier le code pour que la balle se déplace, dépendamment de sa vitesse, de sa direction, et du temps écoulé.

J) D'après l'image ci-dessous, exprimez les coordonnées suivantes suivant les variables du code C++ :

![exo1_coords.png](exo1_coords.png)

> `x1 = g_playerPos.x + kPlayerSize.x`
>
> `x2 = g_ballPos.x`
>
> `x3 = g_ballPos.x + kBallSize.x`
> 
> `x4 = g_aiPos.x + kAiSize.x`
>
> `y1 = g_playerPos.y + kPlayerSize.y`
>
> `y2 = g_playerPos.y`
>
> `y3 = g_aiPos.y + kAiSize.y`
>
> `y4 = g_ballPos.y + kBallSize.y`
>
> `y5 = g_ballPos.y`
>
> `y6 = g_aiPos.y`


K) Implémenter les conditions de détections de collisions.

L) Implémenter les conditions de victoire du joueur et de l'IA.

**Prochain exercice : exo2.md**
