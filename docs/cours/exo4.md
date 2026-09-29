# Exercice 4

**OBJECTIF**  
- Comprendre la mémoire en C++, les pointeurs, les types
- Comprendre ce qu'est un dépassement mémoire, un crash.
- Initiation au chargement de fichiers binaires.


**RESSOURCES**
- [Cours sur les pointeurs (EN)](https://www.gnu.org/software/c-intro-and-ref/manual/html_node/Pointers.html)
- [Autre cours sur les pointeurs (FR)](https://cpp.developpez.com/cours/cpp/?page=4-les-pointeurs-et-references#Lno-VI)

## TD

Le but de ce TD est de charger en RAM des fichiers images BMP et les afficher.

Dans l'explorateur de solutions, à droite, clic-droit sur "Exo 4",
puis clic sur "Définir en tant que projet de démarrage / Set as Startup Project".

A) Prendre connaissance des fichiers fournis et exécutez le programme.
Vous devriez pouvoir utiliser les flèches gauche et droite pour faire défiler des carrés colorés.

Quand le programme est à l'arrêt, passer la souris sur le type `jv::util::Color`,
puis dans la fenêtre qui apparaît, cliquer sur "Disposition de la mémoire".
Dans la fenêtre apparue, alternez entre les deux vues disponibles
en cliquant sur l'icône directement à droite du nom du type.

B) Quelle est la taille du type `jv::util::Color` ?
Revoir la défintion du type dans `jv/jv.h`.
En déduire la différence entre les mots-clés `struct` et `union`.

> Color est un union faisant 4 octets car il prend la taille de son membre le plus grand. 
union est un type qui fait commencer chacun de ses membres avec un offset de 0; les faisant se superposer.
struct est un type où chacun de ses membres est placé à la suite de l'autre en mémoire.

C) À quoi correspond un `std::vector<unsigned char>`?

> unsigned char désigne un emplacement de 1 octet.
std::vector<unsigned char> est donc un tableau contiguë d'octet.

D) À quoi correspond l'astérisque dans la ligne `unsigned char* pFile = file.data()` ?

> C'est un pointeur. Cette variable peut stocker une adresse mémoire vers un objet de type unsigned char (1 octet).

E) Expliquer la syntaxe `uint32_t v1 = *(uint32_t*)(pFile + 0x0012)` ?

> `pFile + 0x0012` : 
pFile est un pointeur d'octet. 0x0012 corespond à 18 en hexadécimal.
Cette opération nous donne l'adresse pointée après un déplacement de 18 fois la taille de son type.
Donc, ici, 18 octets plus loin.

> `(Type)(valeur)` :
C'est un cast explicite. valeur sera interprété comme étant de type Type.
Dans notre cas, notre pointeur d'octet sera maintenant traité comme un pointeur de bloc de 4 octets.

> `*pointeur` :
C'est un déréférencement. La valeur retournée est celle stockée à l'adresse mémoire nommée "pointeur".

> `*(uint32_t*)(pointeur)` :
Ici, on indique que "pointeur" doit etre considèré comme un pointeur de bloc de 4 octets.
Ensuite, grace au déréférencement, on récupere la valeur des 4 octets à cette adresse mémoire.

Mettre un point d'arrêt (breakpoint) sur la ligne définissant `pxSize`.
Lancer le programme, jusqu'à qu'il s'interrompe à ce point d'arrêt.

F) Dans la fenêtre "Espion 1 / Watch 1" en bas, affichez les valeurs numéraires
des expressions suivantes :

> `pFile` : 2382550425728

> `pFile + 1` : 2382550425729 (différence de 1 avec pFile)

> `pPixels` : 2382545725584

> `pPixels + 1` : 2382545725588 (différence de 4 avec pPixels)

G) Pourquoi l'addition `+ 1` donne des résultats différents sur `pFile` et `pPixels` ?

> Car une addition sur une adresse mémoire donne un déplacement par bloc d'octets.
La taille de ce bloc est défini par le type du pointeur.
pFile = 1 octet
pPixels = 4 octets

H) À quoi correspond la syntaxe `pointeur[nombre]` ?

> Elle permet de considérer notre pointeur comme le point de départ d'un tableau.
En C++, cela équivaut à écrire *(pointeur + nombre) . 
C'est l'accès par déréférencement à la valeur pointée par le resultat de notre opération (vue précédemment).

I)  Prenez connaissance de la spécification du format de fichier BMP.

J) Dans le fichier `exo3_image.cpp`, à quoi correspondent les variables :

> `v1` : "Horizontal width of bitmap in pixels"
>
> `v2` : "Vertical height of bitmap in pixels"
>
> `v3` : "Bits Per Pixel" nombre de bits utilisés pour stocker les infos de chaques pixels.

Renommer ces variables de façon appropriée.

K) Implémenter la lecture des données de pixels pour le cas où il y a 24 bits par pixel.

Quelques aides en cas d'erreur :

- Rien ne s'affiche ?
  Bien penser à initialiser le membre "a" des couleurs de pixels.
- Les couleurs ne sont pas les bonnes ?
  Le format de fichier BMP encode les couleurs en BGR :
  un octet pour le bleu d'abord, puis le vert, puis le rouge.
- Il y a un décalage entre plusieurs lignes ?
  Il doit y avoir un problème de calcul de position des pixels,
  soit dans l'image source, soit dans la destination.
- L'image est inversée verticalement ?
  Le format de fichier BMP encode les lignes de la plus basse à la plus haute,
  alors que les données RGBA envoyées au GPU doivent être de haut en bas.

L) Implémenter la lecture des données de pixels pour le cas où il y a 8 bits par pixel.
Dans ce cas, chaque octet de pixel correspond à un index pour référencer
une couleur dans un tableau appelé la "ColorTable".

M) Faire en sorte de remplacer la couleur magenta pur (255, 0, 255, 255)
par une couleur de transparence (0, 0, 0, 255).

**Prochain exercice : exo5.md**