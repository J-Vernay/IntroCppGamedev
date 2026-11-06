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

> La paticularité de code est que template permet n'importe quel type par exemple par exmeple cela pourrait être un int comme une class que l'on a créer

C) Quelle méthode `TestContainer` teste sur chaque conteneur étudié ?

> La méthode resize

D) Que signifie la syntaxe `&v` ?

> Reference de variable v.

E) Analyser la sortie `=== vector-char ===` (première section).
À chaque fois, soustrayez l'adresse de la dernière valeur par l'adresse de la première valeur.
Quel motif observez-vous ?

> Le motif reperer est que en soustrayant les deux nombres j'obtients le parametre n de resize - 1

F) Retrouvez-vous ce motif avec `deque<char>`, `list<char>`, `string` ?

> Oui pour string mais pas pour les autres.

G) Retrouvez-vous ce motif avec `vector<Struct<4096>>` ? Quelle différence ?

> Oui pattern similaire avec juste n - 1 le tout * 4096

H) Assurez-vous d'avoir configuré la compilation avec le profil "Release",
puis exécuter le programme.
Utilisez [CSVPlot](https://www.csvplot.com/) pour charger le fichier CSV le plus récent
dans le dossier `Documents`.

I) En lisant le code de `TestContainer`, de quel code étudie-t-on les performances ?
Combien de mesures sont prises à chaque fois ? Comment sont-elles agrégées ?

> Temps d'iteration et de lecture d'un octet pour chaque donne.
 
I) Mettre "Container" en légende à droite, "TestName" en abscisse, et "IterTimeMs" en ordonnée.
Quel type de conteneur semble particulièrement peu efficace ?

> List de struct<octet>

J) Mettre "TestKind" en légende à droite, "Count" en abscisse, et "IterTimeMs" en ordonnée.
Double-cliquer sur "string" dans la légende pour n'afficher que cette courbe.
Puis cliquer sur "deque-char", "vector-char" et "list-char".
De quel autre conteneur les performances de "string" se rapprochent-elles ?

> Elle se rapproche beaucoup de vector char et un peu de deque char

K) Mettre "TestKind" en légende à droite, "Count" en abscisse, et "IterTimeMs" en ordonnée.
Double-cliquer sur "vector-char" dans la légende pour n'afficher que cette courbe.
Puis cliquer sur "vector-Struct256", "vector-Struct1024", "vector-Struct4096".
Pourquoi les performances de l'itération sur `vector` dépendent du type stocké ?

> Techniquement vu que plus on prend de la place dans la memoire en fonction du nombre d'octet que l'on stock dans 
> dans notre struct. Alors aller cherchez une certaine valeur prend plus de temps car pour la rentrer dans le cache prend plus de temps.

L) Mettre "TestKind" en légende à droite, "Count" en abscisse, et "IterTimeMS" en ordonnée.
Double-cliquer sur "vector-Struct4096" dans la légende pour n'afficher que cette courbe.
Puis cliquer sur "deque-Struct4096" et "list-Struct4096".
Pourquoi les performances de `vector` et `deque` ne sont pas linéaires
par rapport au nombre d'éléments ?

> C'est pas lineaire car ils n ont pas besoins de relire toute les elements comme dans une liste aussi ils sont contigus donc scale bien avec memoire L1/l2/L3
> De plus liste pointe vers une reference et le cache doit aller la chercher donc rajoute latence ce qui n est pas le cas de deque et vector

**Fin des exercices, on attaque le projet, cf. `dino.md`**
