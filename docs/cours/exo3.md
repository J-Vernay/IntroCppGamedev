# Exercice 3

**OBJECTIF**  
- Comprendre le découpage en plusieurs fichiers en C++
- Comprendre la compilation en C++

**RESSOURCES**
- [Cours sur la compilation en C++)](https://laefy.github.io/CPP_Learning/chapter1/5-compilation/)

## TD

Ouvrir le logiciel "Everything" dans le dossier `tools` du dépôt de code.

Dans la barre de menu en haut, dans "Recherche / Search",
cocher "Respecter le chemin / Match path".

A) Chercher `IntroCppGamedev *.h`. Dans quel dossier se trouvent ces fichiers ?

> Dans src puis dans leurs dossiers exo/JV respectifs.

B) Chercher `IntroCppGamedev *.cpp`. Dans quel dossier se trouvent ces fichiers ?

> Dans src puis dans leurs dossiers exo/JV respectifs.

C) Chercher `IntroCppGamedev *.obj`. Dans quel dossier se trouvent ces fichiers ?
Que remarquez-vous des noms des fichiers concernés ?

> Dans debug puis dans leurs dossiers exo/JV respectifs. Ils ont le nom des programmes .cpp que j'ai déjà lancés.

D) Chercher `IntroCppGamedev *.lib`.
Quel(s) fichier(s) apparait(ssent) ?

> JV.lib

Dans la barre de menu de Visual Studio :
- Cliquer sur "Affichage / View" puis "Terminal"
- Entrer la commande `lib /list .\x64-windows\Debug\JV.lib | findstr obj`

E) Que contient le fichier `JV.lib` ?

> Les fichiers .obj du moteur

F) Chercher `IntroCppGamedev !tools *.exe`. 
Dans quel dossier se trouvent ces fichiers ?

> dans build x64 windows Debug 

Dans Visual Studio, dans la fenêtre "Explorateur de solutions / Solution Explorer" à droite :

- Clic-droit sur "Exo2"
- Cliquer sur "Décharger le projet / Unload project"
- Clic-droit sur "Exo2"
- Cliquer sur "Modifier le fichier projet / Edit project file"

Le fichier ainsi affiché sert de configuration à Visual Studio
pour savoir comment compiler notre base de code C++.

G) Où sont définis les dossiers de sortie des fichiers `.obj` et `.exe` ?

> Dans le .vcxproj

H) Où est défini comment est résolu les chemins d'include tel que `<jv/jv.h>` ?

> Dans le .vcxproj avec additionalIncludeDirectories qui va chercher dans src

I) Chercher `LanguageStandard`. Quel est la version du standard C++ que nous utilisons ?

> stdcpp20 (c++ 20 standart)

J) Où sont définis quels fichiers C++ sont compilés pour l'exercice 2 ?

> Dans le .vcxproj
Dans Visual Studio, dans la fenêtre "Explorateur de solutions" à droite :

- Clic-droit sur "Exo2"
- Cliquer sur "Recharger le projet / Reload project"

K) Quels sont les liens entre :

> **Fichiers `.h` et `.cpp` :** Dans le .h, on déclare les variables et fonction et dans le .cpp on les implémentent
>
> **Fichiers `.cpp` et `.obj` :** Les .cpp sont compilés en .obj (fichiers binaires)
>
> **Fichiers `.obj` et `.lib` :** Les .obj constituent les .lib
>
> **Fichiers `.obj` et `.dll` :** Les .obj sont assemblés en .dll
>
> **Fichiers `.obj` et `.exe` :** Un .exe est assemblé de plusieurs .obj par l'éditeur de lien
>
> **Fichiers `.dll` et `.exe` :** Les .dll permettent de donner des fonctions au .exe qui peut les utiliser comme si c'était a lui

L) Quel est le rôle du préprocesseur ?
Comment reconnait-on les directives de préprocesseur ?

> Il permet de préparer toutes les références / includes avant la compilation. On les reconnais car elles commencent par #.

M) Quel est le rôle de l'éditeur de liens ?
Quels sont les deux types de fichiers qu'il peut produire ?
Quelle différence majeure ?

> Il permet de fusionner les .obj en dll et exe. Un exe peut se lancer seul alors qu'un dll est plutot comme une bibliotheque de fonction.

**Prochain exercice : exo4.md**
