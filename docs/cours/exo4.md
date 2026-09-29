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

B) Quelle est la taille du type `jv::util::Color` ?
Revoir la défintion du type dans `jv/jv.h`.
En déduire la différence entre les mots-clés `struct` et `union`.

> Color fait 4 octets. Struct est un type de data "custom", qui peut par exemple stocker
plusieurs float, mais a des adresses mémoires différentes. Union permet de stocker tout ces
float a la même adresse.

C) À quoi correspond un `std::vector<unsigned char>`?

> un unsigned char est une valeur entre 0 et 255, un vector d'unsigned char est une liste de ceux ci.

D) À quoi correspond l'astérisque dans la ligne `unsigned char* pFile = file.data()` ?

> pFile est un pointeur a l'adresse mémoire de l'objet retourné par file.data()

E) Expliquer la syntaxe `uint32_t v1 = *(uint32_t*)(pFile + 0x0012)` ?

> `pFile + 0x0012` : permet de faire un "décallage" dans la mémoire, pFile a sa case mémoire,
 en ajoutant la valeur hexadécimale 0x0012 qui est égale a 18, on accède a la case mémoire qui
 est située 18 octets plus loin.
>
> `(Type)(valeur)` : permet de cast une valeur a un type.
>
> `*pointeur` : récupère la valeur de l'objet pointé?
>
> `*(uint32_t*)(pointeur)` : récupère la valeur pointé par un pointeur, après l'avoir
cast en pointeur de uint32_t.

Quand le programme est à l'arrêt, passer la souris sur le type `jv::util::Color`,
puis dans la fenêtre qui apparaît, cliquer sur "Disposition de la mémoire".
Dans la fenêtre apparue, alternez entre les deux vues disponibles
en cliquant sur l'icône directement à droite du nom du type.

Mettre un point d'arrêt (breakpoint) sur la ligne définissant `pxSize`.
Lancer le programme, jusqu'à qu'il s'interrompe à ce point d'arrêt.

F) Dans la fenêtre "Espion 1 / Watch 1" en bas, affichez les valeurs numéraires
des expressions suivantes :

> `pFile` : BMN-
>
> `pFile + 1` : MN-
>
> `pPixels` : 0x000001edeac55b40 {r=128 '€' g=135 '‡' b=254 'þ' ...}
>
> `pPixels + 1` : 0x000001edeac55b44 {r=253 'ý' g=253 'ý' b=253 'ý' ...}

G) Pourquoi l'addition `+ 1` donne des résultats différents sur `pFile` et `pPixels` ?

> car ce sont des pointeurs, ajouter 1 pointe a une autre case mémoire.

H) À quoi correspond la syntaxe `pointeur[nombre]` ?

> c'est équivalent a faire *(pointeur + nombre).

I)  Prenez connaissance de la spécification du format de fichier BMP.

J) Dans le fichier `exo3_image.cpp`, à quoi correspondent les variables :

> `v1` : Largeur de l'image
>
> `v2` : Hauteur de l'image
>
> `v3` : nombre de Bits par pixels

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