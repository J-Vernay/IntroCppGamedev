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

> `jv.h` : Fait le lien (API) entre le moteur et le/les jeux, défini les fonctions d'interaction avec le GPU, défini les structs (color, vec2...) et namespace importantes (jv, game, gpu...).
>
> `jv_win64_impl.h` : API Interne du moteur, Défini les fonctions de gestion de la fenètre de rendu (Init, Draw, Shut...) et de rendu GPU.
> 
> `jv_win64_main.cpp` : La boucle principale du moteur, appel les fonctions d'initialization de rendu et du jeu, appel la fonction update du jeu, implémente la fonction "GetGamepad" pour lire les inputs du joueur et implémente "Panic" pour terminer le processus en cas d'erreur coté jeu. C'est le fichier "point d'entrée" (WinMain).
>
> `jv_win64_window.cpp` : Implémente les fonctions de gestion de fenètre "InitWindow" et "ShutWindow" et gère les événements de l'OS (Windows) propre au rendu (Destroy, Paint, Size) de la fenètre.
>
> `jv_win64_renderer.cpp` : Implémente les fonctionnalitées de rendu GPU (initialisation des buffers, du renderer...).
>
> `jv_win64_file.cpp` : Implémente la fonction "LoadAsset" qui charge un fichier suivant un chemin d'accès.
>
> `exo1_pong.cpp` : Le fichier principal du jeu "Pong" (exo1), implémente les fonctions de jeu de jv.h (Init, Update, Draw...) et une fonction de rendu de Rectangle.

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

> Init, Update, Shut et Draw.

D) Dans la fonction DrawRect(), que représente 'pos' et 'size' ?

> "size" représente la taille en pixel du rectangle et "pos" la position du rectangle, avec le coin haut gauche comme origine.

E) Qu'est-ce qu'un Vertex ? Que fait `jv::gpu::CreateVertexBuffer()` ?
Que fait `jv::gpu::Draw()` ?

> Un Vertex correspond à un des point qui composent un triangle. "jv::gpu::CreateVertexBuffer()" crée un buffer contenant des vertex et envoie les données au GPU (VRAM). "jv::gpu::Draw()" réalise un "Draw Call" sur le GPU et donne quel buffer/géométrie à rendre avec la texture a appliquer (dans le shader) et un Transform pour positionner l'objet/les vertex dans l'espace.

F) La balle se dirige vers la droite, à une vitesse de 300 pixels par seconde.
Le temps entre deux frames est 20 millisecondes. Quelle distance en pixel a été parcourue entre ces deux frames ?

> 300 * 0.02 (20 millisecondes) = 6 pixels.

G) Le temps entre deux frames est 10 millisecondes. Pendant ce temps, la balle s'est dirigée
suivant le vecteur (-30, 40) (en pixels). Dans quelle direction s'est-elle déplacée ?
À quelle vitesse, en pixels par seconde, cela correspond-il ?

> Vers le coin bas-gauche de l'écran, suivant le vecteur (-0.6,0.8), à une vitesse de 5 000 pixels par secondes.

H) La balle est à la position (100, 200). Elle se dirige en diagonale droite-haut,
à la vitesse de 100 pixels par seconde. À quelle position la balle est-elle au bout d'une seconde ?

> Vecteur de direction (normalisé) = (sqrt(2)/2, -sqrt(2)/2); Position au bout d'une seconde (environ) (170.71, 129.28)

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
