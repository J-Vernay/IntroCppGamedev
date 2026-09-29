# Exercice 6

**OBJECTIF**  
- Première découverte des templates
- Comprendre les différences mémoire des conteneurs.

**RESSOURCES**
- [Containers](https://en.cppreference.com/cpp/container)
- [Dessin de statistiques](https://www.csvplot.com/)

## TD


Le but de ce TD est de comparer le comportement de différents conteneurs en mémoire et en performance.

Dans l'explorateur de solutions, à droite, clic-droit sur "Exo 6",
puis clic sur "Définir en tant que projet de démarrage / Set as Startup Project".

A) Prendre connaissance des fichiers fournis et exécutez le programme.
Vous devriez avoir la console qui apparaît avec diverses informations pour plusieurs conteneurs.

B) Quelle est la particularité de la fonction `TestContainer` ?
Que déclare le code `template <int N> struct Struct` ?

> Un struct avec un tableau de char de taile N.

C) Quelle méthode `TestContainer` teste sur chaque conteneur étudié ?

> resize()

D) Que signifie la syntaxe `&v` ?

> Une valeur du tableau générique

E) Analyser la sortie `=== vector-char ===` (première section).
À chaque fois, soustrayez l'adresse de la dernière valeur par l'adresse de la première valeur.
Quel motif observez-vous ?

> La différence entre ces deux valeurs est de <taille du vecteur> - 1.

F) Retrouvez-vous ce motif avec `deque<char>`, `list<char>`, `string` ?

> Non, non, oui. 

G) Retrouvez-vous ce motif avec `vector<Struct<4096>>` ? Quelle différence ?

> ...

H) Assurez-vous d'avoir configuré la compilation avec le profil "Release",
puis exécuter le programme.
Utilisez [CSVPlot](https://www.csvplot.com/) pour charger le fichier CSV le plus récent
dans le dossier `Documents`.

I) En lisant le code de `TestContainer`, de quel code étudie-t-on les performances ?
Combien de mesures sont prises à chaque fois ? Comment sont-elles agrégées ?

> ...
 
I) Mettre "Container" en légende à droite, "TestName" en abscisse, et "IterTimeMs" en ordonnée.
Quel type de conteneur semble particulièrement peu efficace ?

> ...

J) Mettre "TestKind" en légende à droite, "Count" en abscisse, et "IterTimeMs" en ordonnée.
Double-cliquer sur "string" dans la légende pour n'afficher que cette courbe.
Puis cliquer sur "deque-char", "vector-char" et "list-char".
De quel autre conteneur les performances de "string" se rapprochent-elles ?

> ...

K) Mettre "TestKind" en légende à droite, "Count" en abscisse, et "IterTimeMs" en ordonnée.
Double-cliquer sur "vector-char" dans la légende pour n'afficher que cette courbe.
Puis cliquer sur "vector-Struct256", "vector-Struct1024", "vector-Struct4096".
Pourquoi les performances de l'itération sur `vector` dépendent du type stocké ?

> ...

L) Mettre "TestKind" en légende à droite, "Count" en abscisse, et "IterTimeMS" en ordonnée.
Double-cliquer sur "vector-Struct4096" dans la légende pour n'afficher que cette courbe.
Puis cliquer sur "deque-Struct4096" et "list-Struct4096".
Pourquoi les performances de `vector` et `deque` ne sont pas linéaires
par rapport au nombre d'éléments ?

> ...

**Fin des exercices, on attaque le projet, cf. `dino.md`**
