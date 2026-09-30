# Exercice 5 - Analyse

**OBJECTIF**  
- S'intéresser aux statistiques
- Analyser et comprendre les performances d'un programme C++

**RESSOURCES**  
- [Dessin de statistiques](https://www.csvplot.com/)

## TD - Configuration Debug et Release

On reste encore sur le projet "Exo 5" pour cette partie.

A) Exécutez le programme en vérifiant que "Debug" est bien sélectionné en haut dans Visual Studio, à gauche de "x64-windows".
Notez le temps d'exécution moyen de GetSortedWordCount(), en millisecondes (affiché à la fin de la console).

> ... 3.96491ms

B Sélectionner "Release" à la place de "Debug", et notez le temps d'exécution moyen de GetSortedWordCount().
Pourquoi y a-t-il une différence ? Laisser en "Release" pour la suite.

> ... 0.614011ms Le mode debug prend du temps pour analiser quelles parties du code consomme beaucoup de performances
	alors que le mode release éxécute simplement le code.

C) Exécutez le programme 5 fois, en notant à chaque fois le temps d'exécution moyen (toutes pièces confondues)
de GetSortedWordCount(), en millisecondes (affiché à la fin de la console).
Quelle est l'ordre de grandeur de la variabilité ?

> 0.575661ms     0.574261ms       0.560918ms         0.558214ms     0.586229ms
 0.03 ms

Il est donc important de se mettre en Release pour mesurer les performances,
et d'observer plusieurs fois le comportement du programme avant de conclure sur les performances.

## TD - Analyse de graphes

**IMPORTANT**  
Dans cette partie, pour interpréter les graphes tracés avec CSVPlot,
se référer aux explications dans `aide_analyse_donnees.md`.

À chaque exécution du programme, un fichier CSV est généré dans votre dossier "Documents".
Utilisez [CSVPlot](https://www.csvplot.com/) pour charger le fichier CSV le plus récent.

D) En glissant-déposant, mettez "Nom" en légende à droite,
et "NbCaracteres" à la fois en abscisse (en bas) et en ordonnée (à gauche).
Quel est le top 3 des pièces de théâtre ayant le plus de caractères ?

> le tartuffe, l'étourdi et l'avare

E) Mettre "NbCaracteres" en ordonnée et "NbMots" en abscisse.
Quel type de lien relie ces deux variables ?

> Linéaire

F) En moyenne, combien y a-t-il de caractères par mot ?

> entre 5 et 6

G) Mettre "NbMotsUniques" en ordonnée et "NbMots" en abscisse.
Quel type de lien relie ces deux variables ? Pourquoi ?

> Sous-linéaire car plus il y a de mots, plus il y a de chances que ces mots soient déja apparus.

H) Mettre "msCountLetters" en ordonnée et "NbCaracteres" en abscisse.
Quel type de lien relie ces deux variables ? Pourquoi ?

> Plutot linéaire, le temps d'éxecution est proportionel au nombre de caractères.

I) Mettre "msFindUniqueWords" en ordonnée et "NbMots" en abscisse.
Quel type de lien relie ces deux variables ? Pourquoi ?

> Plutot linéaire, même raison.

## TD - Analyse des performances d'un programme

**IMPORTANT**  
Dans cette partie, nous ré-examinons le temps d'exécution moyen de GetSortedWordCount()
tel qu'affiché directement dans la console (nous n'utilisons plus CSV)
se référer aux explications dans `aide_analyse_donnees.md`.

J) Dans les définitions de types dans `exo5_stats.h`, remplacer la définition
`using String = std::string_view;` par `using String = std::string;`.
Relancez le programme et observez le temps d'exécution moyen de GetSortedWordCount().
Comparer avec les résultats obtenues à la question C.

> 0.760082ms environ 50% plus lent.

K) Remettez `using String = std::string_view;`. Dans `exo5::FindAllPlays()`,
remplacez `String remaining{moliere};` par `std::string remaining{moliere};`.
Pourquoi le logiciel crashe-t-il ?

> Les fonctions de stats utililisent un string view

L) Expliquer la différence entre `std::string_view` et `std::string`.

> Un string "contient" son texte est peut être modifié alors qu'un string view est une copie d'un text grace a des pointeurs et ne peut pas etre modifié.

M) Comparer le temps d'exécution moyen de FindUniqueWords() et GetSortedWordCount().
Est-ce surprenant ? Pourquoi ?

> ...  0.558957ms    0.606414ms  Les deux ont une complexité similaire, la seconde doit en plus trier ce qui prend du temps également.

N) Dans la fonction `main()` inverser l'ordre des appels de fonctions à FindUniqueWords()
et GetSortedWordCount(), puis regarder leur temps d'exécution moyen.
Comment la différence s'explique-t-elle ?

>  0.631282ms    0.699179ms   La première fonction charge les données de stat.words dans la mémoire cache, la deuxième y accède directement.

O) Dans les définitions de types dans `exo5_stats.h`, remplacer les définitions de types
utilisant `std::vector` par `std::deque`.
Quel est le temps d'exécution moyen de GetSortedWordCount() ?

> 0.870925ms

P) Dans les définitions de types dans `exo5_stats.h`, remplacer les définitions de types
utilisant `std::deque` par `std::list`. Les appels à `std::sort()` ne fonctionneront plus.
Remplacez `std::sort(res.begin, res.end(), FUNC)` par `res.sort(FUNC)`.
Quel est le temps d'exécution moyen de GetSortedWordCount() ?

> ...1.00398ms

Conclusion : beaucoup de paramètres peuvent affecter les performances d'un programme.
Le choix d'un conteneur a aussi des implications dans ses performances.
L'exercice suivant étudie les différents comportements des conteneurs.


**Prochain exercice : exo6.md**