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
> a) Classe 2
> b) Méthode 4
> c) Attribut 5
> d) Instance 3
> e) Constructeur 1
> 
> DEFINITIONS :
> 1. Fonction membre responsable d'initialiser l'état
> 2. Type encapsulant un comportement et l'état nécessaire à ce comportement
> 3. Zone mémoire allouée pour stocker l'état d'une classe
> 4. Fonction membre qui peut accéder implicitement à l'état d'une classe
> 5. Variable membre faisant partie de l'état d'une classe
> 
> EXEMPLES :
> I. `Pong::Pong`
> II.  `Pong`
> III. `g_Pong`
> IV. `Pong::Update`
> V.  `Pong::m_color`
> 
> ...

C) Changer le code de `Pong::Draw()` pour modifier `m_color`.
Pourquoi le compilateur émet une erreur ?
Pourquoi cette fonctionnalité du C++ est désirable ?
	A la methode est const et modifier sa valeur non static

> ...

D) Appeler la fonction `exo2::DrawRect()` depuis la fonction `Pong::Draw()`.
Pourquoi le compilateur émet une erreur ?
Qu'est-ce qu'il manque ?

> ...

E) Modifier `exo2_draw.cpp` pour que cela fonctionne.

F) Migrer le code de l'exercice 1 `exo1_main.cpp` vers l'exercice 2 dans `exo2_pong.cpp`.
	A 

G) Découper `Pong::Update()` en trois sous-fonctions :
`_UpdateAI()`, `_UpdatePlayer()` et `_UpdateBall()`.

H) À quoi servent les modificateurs d'accès `public` et `private` ?

> ...

I) Définissez les termes suivants :

> Encapsulation : ...
>
> Invariant d'une classe : ...

J) Implémentez la fonction `exo2::DrawScore()` dans `exo2_draw.cpp`,
puis servez-vous en pour afficher le score du joueur et de l'IA.
Par exemple, sur l'image ci-dessous, le score est de 1-3 en faveur de l'IA.

![exo2_score.png](exo2_score.png)

**Prochain exercice : exo3.md**
