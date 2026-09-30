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

> TestContainer est une fonction template.
>
> `template <int N> struct Struct` déclare un template de struct nommée 'struct' qui prend en paramètre un int (N).

C) Quelle méthode `TestContainer` teste sur chaque conteneur étudié ?

> `TestContainer` teste la méthode .resize() et .clear().

D) Que signifie la syntaxe `&v` ?

> C'est une déclaration par référence.

E) Analyser la sortie `=== vector-char ===` (première section).
À chaque fois, soustrayez l'adresse de la dernière valeur par l'adresse de la première valeur.
Quel motif observez-vous ?

> La différence d'adresse mémoire est égale à la taille du vecteur - 1.

F) Retrouvez-vous ce motif avec `deque<char>`, `list<char>`, `string` ?

> Non, la différence d'adresse est plus grande, sauf pour string qui est identique à `vector<char>`.

G) Retrouvez-vous ce motif avec `vector<Struct<4096>>` ? Quelle différence ?

> Le motif est similaire à vector-char, à la différence que le delta d'adresse mémoire est multiplié par la taille des objets (en l'occurence 4096 bytes)

H) Assurez-vous d'avoir configuré la compilation avec le profil "Release",
puis exécuter le programme.
Utilisez [CSVPlot](https://www.csvplot.com/) pour charger le fichier CSV le plus récent
dans le dossier `Documents`.

I) En lisant le code de `TestContainer`, de quel code étudie-t-on les performances ?
Combien de mesures sont prises à chaque fois ? Comment sont-elles agrégées ?

> On étudie la performance de la lecture dans un for loop de chaque container. 3 mesures sont prises. La valeur minimum est prise.
 
I) Mettre "Container" en légende à droite, "TestName" en abscisse, et "IterTimeMs" en ordonnée.
Quel type de conteneur semble particulièrement peu efficace ?

> Les `list`.

J) Mettre "TestKind" en légende à droite, "Count" en abscisse, et "IterTimeMs" en ordonnée.
Double-cliquer sur "string" dans la légende pour n'afficher que cette courbe.
Puis cliquer sur "deque-char", "vector-char" et "list-char".
De quel autre conteneur les performances de "string" se rapprochent-elles ?

> `string` se rapproche de `vector<char>`

K) Mettre "TestKind" en légende à droite, "Count" en abscisse, et "IterTimeMs" en ordonnée.
Double-cliquer sur "vector-char" dans la légende pour n'afficher que cette courbe.
Puis cliquer sur "vector-Struct256", "vector-Struct1024", "vector-Struct4096".
Pourquoi les performances de l'itération sur `vector` dépendent du type stocké ?

> `vector` stocke les valeurs à la suite dans la mémoire et le CPU récupère des blocs de mémoire entiers. Donc la taille plus petite du `vector<char>` permets au CPU de récupérer en un seul appel mémoire plus de valeur et donc a besoin de moins d'appels pour itérer.

L) Mettre "TestKind" en légende à droite, "Count" en abscisse, et "IterTimeMS" en ordonnée.
Double-cliquer sur "vector-Struct4096" dans la légende pour n'afficher que cette courbe.
Puis cliquer sur "deque-Struct4096" et "list-Struct4096".
Pourquoi les performances de `vector` et `deque` ne sont pas linéaires
par rapport au nombre d'éléments ?

> ...

**Fin des exercices, on attaque le projet, cf. `dino.md`**
