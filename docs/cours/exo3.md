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

> IntroCppGameDev\src

B) Chercher `IntroCppGamedev *.cpp`. Dans quel dossier se trouvent ces fichiers ?

> ... IntroCppGameDev\src

C) Chercher `IntroCppGamedev *.obj`. Dans quel dossier se trouvent ces fichiers ?
Que remarquez-vous des noms des fichiers concernés ?

> Ils sont dans le dossier `x64-windows\Debug` et les noms sont les mêmes que ceux des fichiers `.cpp` mais avec l'extension `.obj`.

D) Chercher `IntroCppGamedev *.lib`.
Quel(s) fichier(s) apparait(ssent) ?
	jv.lib

Dans la barre de menu de Visual Studio :
- Cliquer sur "Affichage / View" puis "Terminal"
- Entrer la commande `lib /list .\x64-windows\Debug\JV.lib | findstr obj`

E) Que contient le fichier `JV.lib` ?
	J'ai l'impression qu'il prend tous les fichiers `.obj` de la compilation et les met dans un seul fichier `.

F) Chercher `IntroCppGamedev !tools *.exe`. 
Dans quel dossier se trouvent ces fichiers ?

Ils sont dans `x64-windows\Debug`

Dans Visual Studio, dans la fenêtre "Explorateur de solutions / Solution Explorer" à droite :

- Clic-droit sur "Exo2"
- Cliquer sur "Décharger le projet / Unload project"
- Clic-droit sur "Exo2"
- Cliquer sur "Modifier le fichier projet / Edit project file"

Le fichier ainsi affiché sert de configuration à Visual Studio
pour savoir comment compiler notre base de code C++.

G) Où sont définis les dossiers de sortie des fichiers `.obj` et `.exe` ?

Ils sont défini dans `<OutDir>$(ProjectDir)x64-windows\Debug\</OutDir>`

H) Où est défini comment est résolu les chemins d'include tel que `<jv/jv.h>` ?

<AdditionalIncludeDirectories>..\src;%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>

I) Chercher `LanguageStandard`. Quel est la version du standard C++ que nous utilisons ?

Version cpp 20

J) Où sont définis quels fichiers C++ sont compilés pour l'exercice 2 ?

<ClInclude Include="..\src\exo2\exo2_draw.h" />
    <ClInclude Include="..\src\exo2\exo2_pong.h" />
  </ItemGroup>
  <ItemGroup>
    <ClCompile Include="..\src\exo2\exo2_draw.cpp" />
    <ClCompile Include="..\src\exo2\exo2_main.cpp" />
    <ClCompile Include="..\src\exo2\exo2_pong.cpp" />

Dans Visual Studio, dans la fenêtre "Explorateur de solutions" à droite :

- Clic-droit sur "Exo2"
- Cliquer sur "Recharger le projet / Reload project"

K) Quels sont les liens entre :

> **Fichiers `.h` et `.cpp` :** h déclaration cpp definition
>
> **Fichiers `.cpp` et `.obj` :** obj code cpp en code machine
>
> **Fichiers `.obj` et `.lib` :**  rassemble plusieurs fichiers obj en un seul fichier lib
>
> **Fichiers `.obj` et `.dll` :** rassemble plusieur obj et stocke les fonctions partagé
>
> **Fichiers `.obj` et `.exe` :** transformation code machine en code fonctionnel
>
> **Fichiers `.dll` et `.exe` :** dll indique quoi charger et décharger à l'ouverture exe

L) Quel est le rôle du préprocesseur ?
Comment reconnait-on les directives de préprocesseur ?
 Préprocesseur a pour but de faire le linkage avec les includes et autres et condition genre #if.
 On reconnait les directives grâce au symbol #

> ...

M) Quel est le rôle de l'éditeur de liens ?
Quels sont les deux types de fichiers qu'il peut produire ?
Quelle différence majeure ?
 Après compilation il génère dll exe. Assemble les obj en exe/dll
 Exe fait tourner le code alors que le dll lui a le code utiliser pas les programs.

> ...

**Prochain exercice : exo4.md**
