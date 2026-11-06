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

> `jv.h` : est utilisé en tant qu'interface de programmation
>
> `jv_win64_impl.h` : API Interne pour que les fichiers d'implémentation parlent entre eux
> 
> `jv_win64_main.cpp` : contient l'implémentation
>
> `jv_win64_window.cpp` : contient l'implémentation
>
> `jv_win64_renderer.cpp` :contient l'implémentation
>
> `jv_win64_file.cpp` : contient l'implémentation
>
> `exo1_pong.cpp` : Utilisateur / appelant pour l'exercice 1

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

> ...

D) Dans la fonction DrawRect(), que représente 'pos' et 'size' ?

> ...

E) Qu'est-ce qu'un Vertex ? Que fait `jv::gpu::CreateVertexBuffer()` ?
Que fait `jv::gpu::Draw()` ?

> ...

F) La balle se dirige vers la droite, à une vitesse de 300 pixels par seconde.
Le temps entre deux frames est 20 millisecondes. Quelle distance en pixel a été parcourue entre ces deux frames ?

> ...

G) Le temps entre deux frames est 10 millisecondes. Pendant ce temps, la balle s'est dirigée
suivant le vecteur (-30, 40) (en pixels). Dans quelle direction s'est-elle déplacée ?
À quelle vitesse, en pixels par seconde, cela correspond-il ?

> ...

H) La balle est à la position (100, 200). Elle se dirige en diagonale droite-haut,
à la vitesse de 100 pixels par seconde. À quelle position la balle est-elle au bout d'une seconde ?

> ...

I) Modifier le code pour que la balle se déplace, dépendamment de sa vitesse, de sa direction, et du temps écoulé.
	
	//done

J) D'après l'image ci-dessous, exprimez les coordonnées suivantes suivant les variables du code C++ :

![exo1_coords.png](exo1_coords.png)

> `x1 = kPlayerSizex`
>
> `x2 = g_ballPos.x`
>
> `x3 = g_ballPos.x`
> 
> `x4 = g_aiPos.x = kArenaSize.x - kAiSize.x`
>
> `y1 = g_PlayerPos.y`
>
> `y2 = g_PlayerPos.y + kPlayerSize.y`
>
> `y3 = g_aiPos.y`
>
> `y4 = ballPos.y`
>
> `y5 = ballPos.y + kBallSize.y`
>
> `y6 = g_aiPos.y + kAiSize.y`


K) Implémenter les conditions de détections de collisions.
	
	

L) Implémenter les conditions de victoire du joueur et de l'IA.

**Prochain exercice : exo2.md**
