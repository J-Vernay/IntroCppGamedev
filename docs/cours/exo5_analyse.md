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

> 442.017ms

B Sélectionner "Release" à la place de "Debug", et notez le temps d'exécution moyen de GetSortedWordCount().
Pourquoi y a-t-il une différence ? Laisser en "Release" pour la suite.

> 12.2691ms
> Le code est optimisé en Release, alors qu'il ne l'est pas en Debug pour être analysable ligne par ligne.

C) Exécutez le programme 5 fois, en notant à chaque fois le temps d'exécution moyen (toutes pièces confondues)
de GetSortedWordCount(), en millisecondes (affiché à la fin de la console).
Quelle est l'ordre de grandeur de la variabilité ?

> 12.3876ms
> 12.7259ms
> 12.843ms
> 12.3166ms
> 12.3378ms
> L'ordre de grandeur de variabilité est au dixième de milliseconde.

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

> Le Tartuffe ou l'Imposteur
> L'étourdi ou les contre-temps
> L'avare

E) Mettre "NbCaracteres" en ordonnée et "NbMots" en abscisse.
Quel type de lien relie ces deux variables ?

> Évolution linéaire.
> Les mots étant composés de caractères, plus de mot implique plus de caractères.

F) En moyenne, combien y a-t-il de caractères par mot ?

> En moyenne, il y a environ 78 646 caractères et 11 322 mots par pièce ce qui fait 78646/11322 = environ 6.9 caractères par mot.

G) Mettre "NbMotsUniques" en ordonnée et "NbMots" en abscisse.
Quel type de lien relie ces deux variables ? Pourquoi ?

> Évolution sous-linéaire.
> Plus de mots -> plus de mots unique car chaque nouveau mot peut être un mot unique.

H) Mettre "msCountLetters" en ordonnée et "NbCaracteres" en abscisse.
Quel type de lien relie ces deux variables ? Pourquoi ?

> Linéaire.
> Plus de caractères -> plus de temps pour les compter.

I) Mettre "msFindUniqueWords" en ordonnée et "NbMots" en abscisse.
Quel type de lien relie ces deux variables ? Pourquoi ?

> Sur-linéaire.
> Plus de mots -> plus de temps pour compter tous les mots uniques avec double for.

## TD - Analyse des performances d'un programme

**IMPORTANT**  
Dans cette partie, nous ré-examinons le temps d'exécution moyen de GetSortedWordCount()
tel qu'affiché directement dans la console (nous n'utilisons plus CSV)
se référer aux explications dans `aide_analyse_donnees.md`.

J) Dans les définitions de types dans `exo5_stats.h`, remplacer la définition
`using String = std::string_view;` par `using String = std::string;`.
Relancez le programme et observez le temps d'exécution moyen de GetSortedWordCount().
Comparer avec les résultats obtenues à la question C.

> 15.5278ms
> La fonction est légèrement moins performante qu'avec un `std::string_view`.

K) Remettez `using String = std::string_view;`. Dans `exo5::FindAllPlays()`,
remplacez `String remaining{moliere};` par `std::string remaining{moliere};`.
Pourquoi le logiciel crashe-t-il ?

> Le programme n'autorise pas de lire un caractère du string.

L) Expliquer la différence entre `std::string_view` et `std::string`.

> `std::string` gère sa propre chaîne de caractères, `std::string_view` est en lecture seule et renvoie à un espace mémoire qui ne lui appartient pas.

M) Comparer le temps d'exécution moyen de FindUniqueWords() et GetSortedWordCount().
Est-ce surprenant ? Pourquoi ?

> FindUniqueWords() a un temps d'éxecution très proche de GetSortedWordCount().
> Ce n'est pas surprenant, ils itèrent tous les deux sur une liste de mots à chaque mot du texte de manière très similaire et avec la même complexité.

N) Dans la fonction `main()` inverser l'ordre des appels de fonctions à FindUniqueWords()
et GetSortedWordCount(), puis regarder leur temps d'exécution moyen.
Comment la différence s'explique-t-elle ?

> 

O) Dans les définitions de types dans `exo5_stats.h`, remplacer les définitions de types
utilisant `std::vector` par `std::deque`.
Quel est le temps d'exécution moyen de GetSortedWordCount() ?

> 16.6153ms

P) Dans les définitions de types dans `exo5_stats.h`, remplacer les définitions de types
utilisant `std::deque` par `std::list`. Les appels à `std::sort()` ne fonctionneront plus.
Remplacez `std::sort(res.begin, res.end(), FUNC)` par `res.sort(FUNC)`.
Quel est le temps d'exécution moyen de GetSortedWordCount() ?

> 25.9191ms

Conclusion : beaucoup de paramètres peuvent affecter les performances d'un programme.
Le choix d'un conteneur a aussi des implications dans ses performances.
L'exercice suivant étudie les différents comportements des conteneurs.


**Prochain exercice : exo6.md**