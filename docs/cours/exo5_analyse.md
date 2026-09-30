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

> 0.0952036ms

B Sélectionner "Release" à la place de "Debug", et notez le temps d'exécution moyen de GetSortedWordCount().
Pourquoi y a-t-il une différence ? Laisser en "Release" pour la suite.

> 0.0135357ms. 
En mode debug on a accès aux outils par exemple les break points.


C) Exécutez le programme 5 fois, en notant à chaque fois le temps d'exécution moyen (toutes pièces confondues)
de GetSortedWordCount(), en millisecondes (affiché à la fin de la console).
Quelle est l'ordre de grandeur de la variabilité ?

>  0.0987393, 0.146032, 0.0942321, 0.0922393, 0.0934071 .L'écart de variabilité est de environ 0.0537927ms.

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

> l'avare, l'étourdi ou les contre temps, le tartuffe ou l'imposteur

E) Mettre "NbCaracteres" en ordonnée et "NbMots" en abscisse.
Quel type de lien relie ces deux variables ?

> si il y a + de mot il y a + de charactères enregistrés.

F) En moyenne, combien y a-t-il de caractères par mot ?

> en moyenne 5.6 caractères / mots

G) Mettre "NbMotsUniques" en ordonnée et "NbMots" en abscisse.
Quel type de lien relie ces deux variables ? Pourquoi ?

>SubLinear  .+ il y a de mots + il y a de chance d'avoir de mots uniques.

H) Mettre "msCountLetters" en ordonnée et "NbCaracteres" en abscisse.
Quel type de lien relie ces deux variables ? Pourquoi ?

> superLinear.  - il y a de charactères à traités + rapide il sera des les compter.

I) Mettre "msFindUniqueWords" en ordonnée et "NbMots" en abscisse.
Quel type de lien relie ces deux variables ? Pourquoi ?

> superLinear.

## TD - Analyse des performances d'un programme

**IMPORTANT**  
Dans cette partie, nous ré-examinons le temps d'exécution moyen de GetSortedWordCount()
tel qu'affiché directement dans la console (nous n'utilisons plus CSV)
se référer aux explications dans `aide_analyse_donnees.md`.

J) Dans les définitions de types dans `exo5_stats.h`, remplacer la définition
`using String = std::string_view;` par `using String = std::string;`.
Relancez le programme et observez le temps d'exécution moyen de GetSortedWordCount().
Comparer avec les résultats obtenues à la question C.
	
> étant donné que c'est un ptr c'est + léger l'execution est + rapide

K) Remettez `using String = std::string_view;`. Dans `exo5::FindAllPlays()`,
remplacez `String remaining{moliere};` par `std::string remaining{moliere};`.
Pourquoi le logiciel crashe-t-il ?

> ...

L) Expliquer la différence entre `std::string_view` et `std::string`.

> std::string est une classe, std::string_view pointer juste possible de la lire (public get private set).

M) Comparer le temps d'exécution moyen de FindUniqueWords() et GetSortedWordCount().
Est-ce surprenant ? Pourquoi ?

> 0.766214ms vs 0.721548ms 

N) Dans la fonction `main()` inverser l'ordre des appels de fonctions à FindUniqueWords()
et GetSortedWordCount(), puis regarder leur temps d'exécution moyen.
Comment la différence s'explique-t-elle ?

> La différence vient du cache. Pour justifié la premiere fonction charge le cache, et la seconde en profite pour s'exécuter plus vite. Ducoup elle sera systématiquement avantagée.

O) Dans les définitions de types dans `exo5_stats.h`, remplacer les définitions de types
utilisant `std::vector` par `std::deque`.
Quel est le temps d'exécution moyen de GetSortedWordCount() ?

> 0.843745ms Plus lent d'environ 0.12ms

P) Dans les définitions de types dans `exo5_stats.h`, remplacer les définitions de types
utilisant `std::deque` par `std::list`. Les appels à `std::sort()` ne fonctionneront plus.
Remplacez `std::sort(res.begin, res.end(), FUNC)` par `res.sort(FUNC)`.
Quel est le temps d'exécution moyen de GetSortedWordCount() ?

> 0.801250ms

Conclusion : beaucoup de paramètres peuvent affecter les performances d'un programme.
Le choix d'un conteneur a aussi des implications dans ses performances.
L'exercice suivant étudie les différents comportements des conteneurs.


**Prochain exercice : exo6.md**