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
> 1. Fonction membre responsable d'initialiser l'état, Constructeur  `Pong::Pong`
> 2. Type encapsulant un comportement et l'état nécessaire à ce comportement, Classe `Pong`
> 3. Zone mémoire allouée pour stocker l'état d'une classe, Instance `g_Pong`
> 4. Fonction membre qui peut accéder implicitement à l'état d'une classe, Méthode `Pong::Update`
> 5. Variable membre faisant partie de l'état d'une classe,  Attribut  `Pong::m_color`
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

> m_color est une variable privée_

D) Appeler la fonction `exo2::DrawRect()` depuis la fonction `Pong::Draw()`.
Pourquoi le compilateur émet une erreur ?
Qu'est-ce qu'il manque ?

> La fonction n'est pas implémentée et exo2_draw.h n'est pas include

E) Modifier `exo2_draw.cpp` pour que cela fonctionne.

F) Migrer le code de l'exercice 1 `exo1_main.cpp` vers l'exercice 2 dans `exo2_pong.cpp`.

G) Découper `Pong::Update()` en trois sous-fonctions :
`_UpdateAI()`, `_UpdatePlayer()` et `_UpdateBall()`.

H) À quoi servent les modificateurs d'accès `public` et `private` ?

> Les variables et fonctions private d'une classe ne peuvent être utilisés que par l'objet de cette classe.
Les privées peuvent être utilisés par n'importe qui.

I) Définissez les termes suivants :

> Encapsulation : ...
>
> Invariant d'une classe : ...

J) Implémentez la fonction `exo2::DrawScore()` dans `exo2_draw.cpp`,
puis servez-vous en pour afficher le score du joueur et de l'IA.
Par exemple, sur l'image ci-dessous, le score est de 1-3 en faveur de l'IA.

![exo2_score.png](exo2_score.png)

**Prochain exercice : exo3.md**
