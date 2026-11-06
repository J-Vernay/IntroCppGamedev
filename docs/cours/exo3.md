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

> Dans le dossier `\src` du projet.

B) Chercher `IntroCppGamedev *.cpp`. Dans quel dossier se trouvent ces fichiers ?

> Dans le dossier `\src` du projet.

C) Chercher `IntroCppGamedev *.obj`. Dans quel dossier se trouvent ces fichiers ?
Que remarquez-vous des noms des fichiers concernés ?

> Dans le dossier `\build\obj\x64-windows\Debug` du projet.

D) Chercher `IntroCppGamedev *.lib`.
Quel(s) fichier(s) apparait(ssent) ?

> Dans le dossier `\build\x64-windows\Debug` du projet

Dans la barre de menu de Visual Studio :
- Cliquer sur "Affichage / View" puis "Terminal"
- Entrer la commande `lib /list .\x64-windows\Debug\JV.lib | findstr obj`

E) Que contient le fichier `JV.lib` ?

> D'autres fichiers compilés du moteur de jeu.

F) Chercher `IntroCppGamedev !tools *.exe`. 
Dans quel dossier se trouvent ces fichiers ?

> Dans le dossier `\build\x64-windows\Debug` du projet.

Dans Visual Studio, dans la fenêtre "Explorateur de solutions / Solution Explorer" à droite :

- Clic-droit sur "Exo2"
- Cliquer sur "Décharger le projet / Unload project"
- Clic-droit sur "Exo2"
- Cliquer sur "Modifier le fichier projet / Edit project file"

Le fichier ainsi affiché sert de configuration à Visual Studio
pour savoir comment compiler notre base de code C++.

G) Où sont définis les dossiers de sortie des fichiers `.obj` et `.exe` ?

> Dans une balise <OutDir>.

H) Où est défini comment est résolu les chemins d'include tel que `<jv/jv.h>` ?

> Dans des <AdditionalIncludeDirectories>.

I) Chercher `LanguageStandard`. Quel est la version du standard C++ que nous utilisons ?

> C++ 20 (stdcpp20).

J) Où sont définis quels fichiers C++ sont compilés pour l'exercice 2 ?

> Dans des balises <ClCompile>.

Dans Visual Studio, dans la fenêtre "Explorateur de solutions" à droite :

- Clic-droit sur "Exo2"
- Cliquer sur "Recharger le projet / Reload project"

K) Quels sont les liens entre :

> **Fichiers `.h` et `.cpp` :** Les `.h` déclarent, les `.cpp` implémentent.
>
> **Fichiers `.cpp` et `.obj` :** Les `.cpp` sont compilés en `.obj`.
> 
> **Fichiers `.obj` et `.lib` :** Les `.obj` sont répertoriés dans le `.lib`.
>
> **Fichiers `.obj` et `.dll` :** Les `.dll` sont des librairies utilisant des `.obj`.
>
> **Fichiers `.obj` et `.exe` :** Les `.exe` sont des exécutables utilisant des `.obj`.
>
> **Fichiers `.dll` et `.exe` :** Les `.exe` peuvent dépendre de `.dll`.

L) Quel est le rôle du préprocesseur ?
Comment reconnait-on les directives de préprocesseur ?

> Le processeur lit et réécrit les fichiers de code avant la compilation. Les directives du préprocesseur ont comme préfixe `#`.

M) Quel est le rôle de l'éditeur de liens ?
Quels sont les deux types de fichiers qu'il peut produire ?
Quelle différence majeure ?

> L'éditeur de liens crée des exécutables à partir des fichiers objects.
> Il peut produire des `.exe` et des `.dll`.
> Un `.exe` est un programme autonome, alors qu'un `.dll` est une librairie qui peut être appelée par un autre programme.

**Prochain exercice : exo4.md**
