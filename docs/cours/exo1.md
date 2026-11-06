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
> Déclaration du moteur de jeu.
> `jv_win64_impl.h` :
> Déclaration de l'interface entre le moteur et Windows.
> `jv_win64_main.cpp` : 
> Déclaration des méthodes principales relatives à Windows.
> `jv_win64_window.cpp` : 
> Déclaration des méthodes liées à la fenêtre.
> `jv_win64_renderer.cpp` :
> Déclaration des méthodes liées au rendu.
> `jv_win64_file.cpp` : 
> Déclaration de la gestion des fichiers
> `exo1_pong.cpp` : 
> Déclaration d'un jeu (un Pong) utilisant le moteur.

B) Remettez les commentaires ci-dessous aux bons endroits,
dans les fichiers `jv_win64_main.cpp` et `jv_win64_window.cpp`:

> 
>
> 
>
> 
>
> 
>
> 
>
> 
>
> 
>
> 
>
> 
>
> 
>
> 
>
> 
>
> 
>
> 
>
> 
>
> 
>
> 
>
> 

C) Quelles sont les 4 fonctions qu'un jeu doit fournir au moteur de jeu JV ?

> Init, Update, Draw et Shut

D) Dans la fonction DrawRect(), que représente 'pos' et 'size' ?

> pos représente la position du coin haut-gauche du rectangle, size représente sa taille

E) Qu'est-ce qu'un Vertex ? Que fait `jv::gpu::CreateVertexBuffer()` ?
Que fait `jv::gpu::Draw()` ?

> Un vertex est un sommet de segment. `jv::gpu::CreateVertexBuffer()` crée un buffer de vertices envoyés à la carte graphique.
> `jv::gpu::Draw()` utilise ces vertices pour demander à la carte graphique de dessiner des triangles texturés.

F) La balle se dirige vers la droite, à une vitesse de 300 pixels par seconde.
Le temps entre deux frames est 20 millisecondes. Quelle distance en pixel a été parcourue entre ces deux frames ?

> La distance parcourue est de (300/1000)*20 = 6 pixels.

G) Le temps entre deux frames est 10 millisecondes. Pendant ce temps, la balle s'est dirigée
suivant le vecteur (-30, 40) (en pixels). Dans quelle direction s'est-elle déplacée ?
À quelle vitesse, en pixels par seconde, cela correspond-il ?

> Elle s'est déplacé vers le bas-gauche de la fenêtre à une vitesse de sqrt(30²+40²) * 100 = 5000 pixel par seconde.

H) La balle est à la position (100, 200). Elle se dirige en diagonale droite-haut,
à la vitesse de 100 pixels par seconde. À quelle position la balle est-elle au bout d'une seconde ?

> La balle se déplace donc de environ de 100/sqrt(2) = 70.7 pixel par seconde en x et en y, elle se trouve donc en environ (170.7, 29.3).

I) Modifier le code pour que la balle se déplace, dépendamment de sa vitesse, de sa direction, et du temps écoulé.

J) D'après l'image ci-dessous, exprimez les coordonnées suivantes suivant les variables du code C++ :

![exo1_coords.png](exo1_coords.png)

> `x1 = g_playerPos.x + kPlayerSize.x`
>
> `x2 = g_ballPos.x`
>
> `x3 = g_ballPos.x + kBallSize.x`
> 
> `x4 = g_aiPos.x`
>
> `y1 = g_playerPos.y`
>
> `y2 = g_playerPos.y + kPlayerSize.y`
>
> `y3 = g_aiPos.y`
>
> `y4 = g_ballPos.y`
>
> `y5 = g_ballPos.y + kBallSize.y`
>
> `y6 = g_aiPos.y + kAiSize.y`


K) Implémenter les conditions de détections de collisions.

L) Implémenter les conditions de victoire du joueur et de l'IA.

**Prochain exercice : exo2.md**
