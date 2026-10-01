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

> 
Le mot clef template permet de déclarer des modéles génériques.
Ces modéles utilisent des parametres à fournir au compilateur. A chaque utilisation du modéle avec des parametres differents, une nouvelle version est généré.
Cela peut etre des parametres types (où le parametre est remplacé dans le modéle par un type donné).
Ou des parametres non-types (où le parametre est une valeur connue à la compilation).

TestContainer est une fonction générique qui utilise un paramétre type (TContainer). Cela évite la duplication manuelle pour chaque type à tester.

Struct est un struct générique utilisant un paramétre non-type. Il faut obligatoirement que N soit connu à la compilation pour que le compilateur génére les différentes versions.


C) Quelle méthode `TestContainer` teste sur chaque conteneur étudié ?

> Elle teste la méthode resize sur le conteneur avec différentes taille.

D) Que signifie la syntaxe `&v` ?

> La valeur renvoyée est l'adresse mémoire de v.

E) Analyser la sortie `=== vector-char ===` (première section).
À chaque fois, soustrayez l'adresse de la dernière valeur par l'adresse de la première valeur.
Quel motif observez-vous ?

> La différence d'adresse entre la dernière et la première valeur est de N-1 (où N est la taille du conteneur).
Cela s'explique car std::vector est contigu en mémoire et les char font exactement 1 octet chacun.

F) Retrouvez-vous ce motif avec `deque<char>`, `list<char>`, `string` ?

> On retrouve ce motif pour string car ses éléments sont contigus en mémoire (comme vector::std).
Mais pas pour list<char> qui est une list chainée ou deque<char> qui fragmente sa mémoire (il contient des pointeurs vers des espaces mémoire agissant par "chunk").

G) Retrouvez-vous ce motif avec `vector<Struct<4096>>` ? Quelle différence ?

> Struct<4096> contient un tableau continu de 4096 char (donc 4096 octets).
vector est aussi contigu.
Le pattern est un écart équivalent entre chaque adresse comme pour vector<char> mais chaque élément fait ici 4096 octets au lieu de 1.
On obtient une différence entre l'adresse de fin et de début de: (N-1)*4096

H) Assurez-vous d'avoir configuré la compilation avec le profil "Release",
puis exécuter le programme.
Utilisez [CSVPlot](https://www.csvplot.com/) pour charger le fichier CSV le plus récent
dans le dossier `Documents`.

I) En lisant le code de `TestContainer`, de quel code étudie-t-on les performances ?
Combien de mesures sont prises à chaque fois ? Comment sont-elles agrégées ?

> On étudie les performances des boucle for range à parcourir tout les éléments du conteneur.
On prend trois mesures en millisecondes et on retient seulement la plus rapide des trois.
 
I) Mettre "Container" en légende à droite, "TestName" en abscisse, et "IterTimeMs" en ordonnée.
Quel type de conteneur semble particulièrement peu efficace ?

> La list est le type de conteneur prenant le plus de temps à etre parcouru. Ici, il y a un rapport énorme entre le nombre d'éléments et le temps d'execution.

J) Mettre "TestKind" en légende à droite, "Count" en abscisse, et "IterTimeMs" en ordonnée.
Double-cliquer sur "string" dans la légende pour n'afficher que cette courbe.
Puis cliquer sur "deque-char", "vector-char" et "list-char".
De quel autre conteneur les performances de "string" se rapprochent-elles ?

> Elles se rapprochent de vector<char> . Cela s'explique par leur nature commune de bloc contigu d'éléments.

K) Mettre "TestKind" en légende à droite, "Count" en abscisse, et "IterTimeMs" en ordonnée.
Double-cliquer sur "vector-char" dans la légende pour n'afficher que cette courbe.
Puis cliquer sur "vector-Struct256", "vector-Struct1024", "vector-Struct4096".
Pourquoi les performances de l'itération sur `vector` dépendent du type stocké ?

> Grâce au placement contigu des éléments de vector en mémoire, plusieurs de ces éléments peuvent etre placés simultanéments en cache (ce qui rend plus rapide leur accès par la suite).
Cependant, le CPU charge cette mémoire par blocs de taille fixe (lignes de cache).
Généralement, ces lignes de cache font 64 octets.

Pour un bloc de petits éléments comme vector<char>(1 octet), on peut donc mettre 64 éléments en une seule ligne de cache.

Mais pour des gros objets comme Struct4096 (4096 octets), un seul élément demande plusieurs lignes de cache pour atteindre le CPU. 
On perd donc l'avantage du chargement en cache. L'itération est ralentie par la distance physique entre la RAM et le CPU qui reçoit ses données.


L) Mettre "TestKind" en légende à droite, "Count" en abscisse, et "IterTimeMS" en ordonnée.
Double-cliquer sur "vector-Struct4096" dans la légende pour n'afficher que cette courbe.
Puis cliquer sur "deque-Struct4096" et "list-Struct4096".
Pourquoi les performances de `vector` et `deque` ne sont pas linéaires
par rapport au nombre d'éléments ?

> 
La list est parfaitement discontinue en mémoire. 
La boucle prendra ses éléments un par un en suivant les pointeurs. 
Il est donc impossible pour le processeur d'optimiser ses accès par cache; il faudra forcement passer par la RAM à chaque itération. (cache miss)
Le temps de parcours d'une list est donc linéaire par rapport à son nombre d'éléments.

vector et deque sont eux contigu en mémoire. vector stocke de manière contiguë ses éléments et deque stocke de manière contiguë ses redirections vers les blocs contenant ses données.
Pour vector et deque, on observe une courbe sous-linéaire.
Cela est dû à une spécificitée des processeurs modernes; ils anticipent les accès mémoires (fetch) sur des pattern répétés d'itérations dans des espaces contigus (principe de Hardware Prefetching).
Le processeur économise donc du temps d'attente entre chaque "fetch".


**Fin des exercices, on attaque le projet, cf. `dino.md`**
