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

> 171.6 ms

B Sélectionner "Release" à la place de "Debug", et notez le temps d'exécution moyen de GetSortedWordCount().
Pourquoi y a-t-il une différence ? Laisser en "Release" pour la suite.

> 14.8 ms
Il y a une différence supérieure à un facteur x10.
La différence s'explique par la façon dont le compilateur va stocker les variables selon le mode.
En Debug, il veut garder les variables en mémoire sur le Stack pour que le débugger puisse les inspecter facilement. Mais cela à un cout de performance car il faut plus d'instructions au programme pour accéder aux valeurs.
En Release, le compilateur va optimiser les emplacements mémoire. C'est plus rapide pour le programme mais difficile d'accès pour le débugger de notre IDE.

C) Exécutez le programme 5 fois, en notant à chaque fois le temps d'exécution moyen (toutes pièces confondues)
de GetSortedWordCount(), en millisecondes (affiché à la fin de la console).
Quelle est l'ordre de grandeur de la variabilité ?

> 
14.9436ms - 14.9745ms - 15.0692ms - 15.7692ms - 14.5877ms
Il y a une variabilité d'environ 1 milliseconde.
Cela s'explique car en pratique notre programme partage les ressources de l'ordinateur avec toutes les autres applications. Ce qui amene à des variations plus ou moins importantes.

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

> 1) Le Tartuffe - 2) L'étourdi - 3) L'Avare

E) Mettre "NbCaracteres" en ordonnée et "NbMots" en abscisse.
Quel type de lien relie ces deux variables ?

> On voit une évolution linéaire entre le nombre de caractères et le nombre de mots.

F) En moyenne, combien y a-t-il de caractères par mot ?

> On prend un point de la courbe représentatif de la tendance (21k, 120k).
On calcul le coefficient de proportionnalité R = 120000/21000 ≈ 5.7
Il y a en moyenne environ 5.7 charactères par mot.

G) Mettre "NbMotsUniques" en ordonnée et "NbMots" en abscisse.
Quel type de lien relie ces deux variables ? Pourquoi ?

> On a une évolution sous linéaire. 
Cela s'explique car un texte peut contenir autant de mots qu'il veut; alors que le nombre de mots unique est restreint par le nombre de mots de la langue française.

H) Mettre "msCountLetters" en ordonnée et "NbCaracteres" en abscisse.
Quel type de lien relie ces deux variables ? Pourquoi ?

> On obtient un lien globalement linéaire (quelques anomalies existent et peuvent s'expliquer par l'execution d'autres logiciels imprévisible).
Cela s'explique par notre système de compteur de lettres. Il utilise une boucle for range qui effectue un nombre d'itération proportionnel au nombre de caractères à parcourir.

I) Mettre "msFindUniqueWords" en ordonnée et "NbMots" en abscisse.
Quel type de lien relie ces deux variables ? Pourquoi ?

> On observe une évolution sur-linéaire de notre courbe.
Cela s'explique par la structure de notre fonction FindUniqueWords qui utilise 2 boucles for range imbriqués.
La première effectue un nombre d'itération proportionnel au nombre de mots donné.
La deuxième effectue un nombre d'itération proportionnel au nombre de mots uniques deja croisés.
Plus le nombre de mots de départ est grand, plus le nombre de mots uniques sera grand; et donc la deuxième boucle aura de plus en plus d'itérations à effectuer pour chaque mot.


## TD - Analyse des performances d'un programme

**IMPORTANT**  
Dans cette partie, nous ré-examinons le temps d'exécution moyen de GetSortedWordCount()
tel qu'affiché directement dans la console (nous n'utilisons plus CSV)
se référer aux explications dans `aide_analyse_donnees.md`.

J) Dans les définitions de types dans `exo5_stats.h`, remplacer la définition
`using String = std::string_view;` par `using String = std::string;`.
Relancez le programme et observez le temps d'exécution moyen de GetSortedWordCount().
Comparer avec les résultats obtenues à la question C.

> 17.5ms
On avait 14.8ms pour std::string_view et on a 17.5ms pour std::string.
Ce qui nous donne une perte de performance d'environ 20% .

K) Remettez `using String = std::string_view;`. Dans `exo5::FindAllPlays()`,
remplacez `String remaining{moliere};` par `std::string remaining{moliere};`.
Pourquoi le logiciel crashe-t-il ?

> Il y a par exemple un crash au moment d'affecter remaining.substr() (qui renvoie un string temporaire dans ce cas) à play.name (string_view).
string_view fonctionne par pointeur, on demande donc à play.name de pointer vers un string temporaire.
Au moment où ce string est détruit en mémoire, play.name pointe vers un espace mémoire libéré et crash car il n'en a pas l'accès.
En temps normal, il n'y a pas ce problème car substr() d'un string_view renvoi un string_view sans allocation suplémentaire. Donc play.name va directement etre affécté par la valeur de retour.

L) Expliquer la différence entre `std::string_view` et `std::string`.

> std::string est proprétaire de l'espace mémoire qu'il utilise pour stocker sa chaine de caractères.
std::string_view est non-propriétaire, il référence juste une chaine de charactére contenue dans un autre espace mémoire (grâce à un pointeur et une taille).

M) Comparer le temps d'exécution moyen de FindUniqueWords() et GetSortedWordCount().
Est-ce surprenant ? Pourquoi ?

> 
Moyenne msFindUniqueWords: 14.6ms
Moyenne msGetSortedWordCount: 14.8ms
Au premier abord, on peut trouver cela surprennant car l'une effectue un travail suplémentaire pour compter et faire le tri.
Mais cela se comprend car la plupart du temps d'execution est dû au parcours d'un tableau par double boucle for.
Le reste est négligeable.


N) Dans la fonction `main()` inverser l'ordre des appels de fonctions à FindUniqueWords()
et GetSortedWordCount(), puis regarder leur temps d'exécution moyen.
Comment la différence s'explique-t-elle ?

> La différence de perf peut s'expliquer par plusieurs mécanismes Hardware; qui, selon l'ordre d'execution, peuvent gérer différemment les sytèmes de cache, prefetching, ...

O) Dans les définitions de types dans `exo5_stats.h`, remplacer les définitions de types
utilisant `std::vector` par `std::deque`.
Quel est le temps d'exécution moyen de GetSortedWordCount() ?

> 15.9ms (environ 1ms de plus qu'avec vector)

P) Dans les définitions de types dans `exo5_stats.h`, remplacer les définitions de types
utilisant `std::deque` par `std::list`. Les appels à `std::sort()` ne fonctionneront plus.
Remplacez `std::sort(res.begin, res.end(), FUNC)` par `res.sort(FUNC)`.
Quel est le temps d'exécution moyen de GetSortedWordCount() ?

> 31.4ms (environ 15ms de plus qu'avec vector)

Conclusion : beaucoup de paramètres peuvent affecter les performances d'un programme.
Le choix d'un conteneur a aussi des implications dans ses performances.
L'exercice suivant étudie les différents comportements des conteneurs.


**Prochain exercice : exo6.md**