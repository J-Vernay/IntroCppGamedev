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
puis clic sur "Définir en tant que projet de démarrage".

A) Prendre connaissance des fichiers fournis et exécutez le programme.
Vous devriez pouvoir utiliser les flèches gauche et droite pour faire défiler des carrés colorés.

B) Quelle est la taille du type `jv::util::Color` ?
Revoir la défintion du type dans `jv/jv.h`.
En déduire la différence entre les mots-clés `struct` et `union`.

> jv::util::Color fait 4 octets. Union ça allour un seul espace pour tous ses membres alors que struct ça garde un espace mémoire pour chaques variable 

C) À quoi correspond un `std::vector<unsigned char>`?

> C'est un tableau de caractères (char) non signés.

D) À quoi correspond l'astérisque dans la ligne `unsigned char* pFile = file.data()` ?

> C'est un pointeur.

E) Expliquer la syntaxe `uint32_t v1 = *(uint32_t*)(pFile + 0x0012)` ?

> `pFile + 0x0012` : Pointeur vers le 18e octet
>
> `(Type)(valeur)` : C'est une multiplication
>
> `*pointeur` : C'est pour déréférencer. Pour récupérer la valeur que pointe le pointeur
>
> `*(uint32_t*)(pointeur)` : Pointeur vers uint 32 a l'emplacement du 18e octet 

Quand le programme est à l'arrêt, passer la souris sur le type `jv::util::Color`,
puis dans la fenêtre qui apparaît, cliquer sur "Disposition de la mémoire".
Dans la fenêtre apparue, alternez entre les deux vues disponibles
en cliquant sur l'icône directement à droite du nom du type.

Mettre un point d'arrêt (breakpoint) sur la ligne définissant `pxSize`.
Lancer le programme, jusqu'à qu'il s'interrompe à ce point d'arrêt.

F) Dans la fenêtre "Espion 1 / Watch 1" en bas, affichez les valeurs numéraires
des expressions suivantes :

> `pFile` : "BMN-\x15"
>
> `pFile + 1` : "MN-\x15"
>
> `pPixels` : r = 128 , g = 75, b =11 
>
> `pPixels + 1` : r = 253, g = 253, b = 253

G) Pourquoi l'addition `+ 1` donne des résultats différents sur `pFile` et `pPixels` ?

> Parce que on déplace l'endroit où on regarde dans la mémoire.

H) À quoi correspond la syntaxe `pointeur[nombre]` ?

> C'est une addition

I)  Prenez connaissance de la spécification du format de fichier BMP.

J) Dans le fichier `exo3_image.cpp`, à quoi correspondent les variables :

> `v1` : La largeure horizontale de la bitmap
>
> `v2` : La hauteur verticale de la bitmap
>
> `v3` : Bits par pixels

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