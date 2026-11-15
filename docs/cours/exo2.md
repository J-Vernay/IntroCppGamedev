# Exercice 2

**OBJECTIF**  
- Comprendre le découpage en plusieurs fichiers en C++.
- Première découverte de la programmation orientée objet (POO) en C++.

**RESSOURCES**
- [Cours sur la POO en C++)](https://zestedesavoir.com/tutoriels/822/la-programmation-en-c-moderne/la-programmation-orientee-objet/)

## TD

Dans ce TD, nous allons adapter le code de Pong de l'exercice 1 sous forme
de programmation orientée objet.

Dans l'explorateur de solutions, à droite, clic-droit sur "Exo 2",
puis clic sur "Définir en tant que projet de démarrage".

A) Prendre connaissance des fichiers fournis et exécutez le programme.
Vous devriez voir une fenêtre qui change de couleur.

B) Assembler le vocabulaire de POO avec leurs définitions
et les exemples dans le code :

> VOCABULAIRE :
> a) Classe
> b) Méthode
> c) Attribut
> d) Instance
> e) Constructeur
> 
> DEFINITIONS :
> 1. Fonction membre responsable d'initialiser l'état                                   reponse E
> 2. Type encapsulant un comportement et l'état nécessaire à ce comportement            reponse A
> 3. Zone mémoire allouée pour stocker l'état d'une classe                              reponse D
> 4. Fonction membre qui peut accéder implicitement à l'état d'une classe               reponse B
> 5. Variable membre faisant partie de l'état d'une classe                              reponse C
> 
> EXEMPLES :
> I. `Pong::m_color`
> II. `Pong::Update`
> III. `g_Pong`
> IV. `Pong`
> V. `Pong::Pong`
> 
> ...

C) Changer le code de `Pong::Draw()` pour modifier `m_color`.
Pourquoi le compilateur émet une erreur ?
Pourquoi cette fonctionnalité du C++ est désirable ?

> Le compilateur émet une erreur car la fonction est en const donc on ne peut pas la modifier 

D) Appeler la fonction `exo2::DrawRect()` depuis la fonction `Pong::Draw()`.
Pourquoi le compilateur émet une erreur ?
Qu'est-ce qu'il manque ?

> ya aucune definition de drawrect dans le cpp donc il ne le trouve car il est declare dans le .h ais non trouver dans la cpp car pas defini

E) Modifier `exo2_draw.cpp` pour que cela fonctionne.

F) Migrer le code de l'exercice 1 `exo1_main.cpp` vers l'exercice 2 dans `exo2_pong.cpp`.

G) Découper `Pong::Update()` en trois sous-fonctions :																	 
`_UpdateAI()`, `_UpdatePlayer()` et `_UpdateBall()`.																	 
																														 
H) À quoi servent les modificateurs d'accès `public` et `private` ?														 
																														 
> 	ils servent a savoir si on peut acceder ou non au variable dans la classe ou dans toute la solution																			 
																														 
I) Définissez les termes suivants :																						 
																														 
> Encapsulation : ce qui est encapsule sont les variables membres, l'exterieur peut acceder au variable membre qui sont en public (cacher etat interne d'une classe pour n'exposer que son comportement)																								 
>																														 
> Invariant d'une classe : propriete ou condition toujours vrai sur etat interne de la classe ( quand on lit l'etat interne on a une idée plus precise de ce qu'on attend et si il est faux bug dans méthode qui modif etat)
Interet constructeur = toujours appelé et responsable d'initialiser l'état de façon à respecter les invariants
																														 
J) Implémentez la fonction `exo2::DrawScore()` dans `exo2_draw.cpp`,													 
puis servez-vous en pour afficher le score du joueur et de l'IA.														 
Par exemple, sur l'image ci-dessous, le score est de 1-3 en faveur de l'IA.												 
																														 
![exo2_score.png](exo2_score.png)																						 
																														 
**Prochain exercice : exo3.md**																							 
																														 