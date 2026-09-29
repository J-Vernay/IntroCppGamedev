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

> La taille du type jv::util::Color est de 4 octets
La différence entre une struct  et union est la gestion de mémoire une struct alloue un espace memoire pour chaqu'un de ses membres alors que union alloue un seul espace mémoire pour tous ses membres

C) À quoi correspond un `std::vector<unsigned char>`?

> tableau de char qui est un octet = 8 bits donc 256 valeurs entre [-128;127] qui est signé ou alors [0;255] qui est non signé 

D) À quoi correspond l'astérisque dans la ligne `unsigned char* pFile = file.data()` ?

> un pointeur

E) Expliquer la syntaxe `uint32_t v1 = *(uint32_t*)(pFile + 0x0012)` ?

> `pFile + 0x0012` : pointeur + nombre = aller n cases plus loin 
>
> `(Type)(valeur)` : Pointeur vers un uint32 a l'emplacement du 18eme octet
>
> `*pointeur` : lire la valeur de ce type stocker a cet endroit
>
> `*(uint32_t*)(pointeur)` : lire un uint32 a l'emplacement du 18eme octet

Quand le programme est à l'arrêt, passer la souris sur le type `jv::util::Color`,
puis dans la fenêtre qui apparaît, cliquer sur "Disposition de la mémoire".
Dans la fenêtre apparue, alternez entre les deux vues disponibles
en cliquant sur l'icône directement à droite du nom du type.

Mettre un point d'arrêt (breakpoint) sur la ligne définissant `pxSize`.
Lancer le programme, jusqu'à qu'il s'interrompe à ce point d'arrêt.

F) Dans la fenêtre "Espion 1 / Watch 1" en bas, affichez les valeurs numéraires
des expressions suivantes :

> `pFile` : 0x000002136ac1b080 "BMN-\x15"
>
> `pFile + 1` : 0x000002136ac1b081 "MN-\x15"
>
> `pPixels` : 0x000002136aa56730 {r=128 '€' g=75 'K' b=11 '\v' ...}
>
> `pPixels + 1` : 0x000002136aa56734 {r=253 'ȳ' g=253 'ȳ' b=253 'ȳ' ...}

G) Pourquoi l'addition `+ 1` donne des résultats différents sur `pFile` et `pPixels` ?

> car pFile et pPixels n'est pas la meme chose ça vient du type un char vaut 1 et un color vaut 4.

H) À quoi correspond la syntaxe `pointeur[nombre]` ?

> c'est vouloir acceder a la case 0 a partir de pPixels

I)  Prenez connaissance de la spécification du format de fichier BMP.

J) Dans le fichier `exo3_image.cpp`, à quoi correspondent les variables :

> `v1` : Widthpx
>
> `v2` : Heightpx
>
> `v3` : Bits par pixels comment pixels code dans fichier

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