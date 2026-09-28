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

> IntroCppGamedev\src

B) Chercher `IntroCppGamedev *.cpp`. Dans quel dossier se trouvent ces fichiers ?

> IntroCppGamedev\src

C) Chercher `IntroCppGamedev *.obj`. Dans quel dossier se trouvent ces fichiers ?
Que remarquez-vous des noms des fichiers concernés ?

> IntroCppGamedev\build\obj\x64-windows\Debug

D) Chercher `IntroCppGamedev *.lib`.
Quel(s) fichier(s) apparait(ssent) ?

> IntroCppGamedev\build\obj\x64-windows\Debug

Dans la barre de menu de Visual Studio :
- Cliquer sur "Affichage / View" puis "Terminal"
- Entrer la commande `lib /list .\x64-windows\Debug\JV.lib | findstr obj`

E) Que contient le fichier `JV.lib` ?

> il contient les chemins d'accès du code compilé de la librairie JV

F) Chercher `IntroCppGamedev !tools *.exe`. 
Dans quel dossier se trouvent ces fichiers ?

> IntroCppGamedev\build\obj\x64-windows\Debug

Dans Visual Studio, dans la fenêtre "Explorateur de solutions / Solution Explorer" à droite :

- Clic-droit sur "Exo2"
- Cliquer sur "Décharger le projet / Unload project"
- Clic-droit sur "Exo2"
- Cliquer sur "Modifier le fichier projet / Edit project file"

Le fichier ainsi affiché sert de configuration à Visual Studio
pour savoir comment compiler notre base de code C++.

G) Où sont définis les dossiers de sortie des fichiers `.obj` et `.exe` ?

> <OutDir>$(ProjectDir)x64-windows\Debug\</OutDir>

H) Où est défini comment est résolu les chemins d'include tel que `<jv/jv.h>` ?

><AdditionalIncludeDirectories>..\src;%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>

I) Chercher `LanguageStandard`. Quel est la version du standard C++ que nous utilisons ?

> version 20

J) Où sont définis quels fichiers C++ sont compilés pour l'exercice 2 ?

> <ItemGroup>
    <ClCompile Include="..\src\exo2\exo2_draw.cpp" />
    <ClCompile Include="..\src\exo2\exo2_main.cpp" />
    <ClCompile Include="..\src\exo2\exo2_pong.cpp" />
  </ItemGroup>

Dans Visual Studio, dans la fenêtre "Explorateur de solutions" à droite :

- Clic-droit sur "Exo2"
- Cliquer sur "Recharger le projet / Reload project"

K) Quels sont les liens entre :

> **Fichiers `.h` et `.cpp` :** 
>le header déclare les fonctions, le cpp implémente ces fonctions
> **Fichiers `.cpp` et `.obj` :** ...
>le obj est le cpp compilé
> **Fichiers `.obj` et `.lib` :** ...
>le lib est une liste de obj
> **Fichiers `.obj` et `.dll` :** ...
>un dll est une combinaise de fonctions contenue dans les obj prête a être exécutée
> **Fichiers `.obj` et `.exe` :** ...
>le exe est le résultat final de l'éditeur de lien, qui relie les obj
> **Fichiers `.dll` et `.exe` :** 
l'exe appelle les fonctions contenu dans le dll

L) Quel est le rôle du préprocesseur ?
Comment reconnait-on les directives de préprocesseur ?

> il exécute les instructions qui commence avec # comme les include, pour lesquels il
remplace l'include par le contenu du fichier inclu

M) Quel est le rôle de l'éditeur de liens ?
Quels sont les deux types de fichiers qu'il peut produire ?
Quelle différence majeure ?

> il créé des liens entre les fonctions, qui améliore le parcours d'une fonction a une autre
et d'ignorer les fonctions qui ne sont pas utiles
Le .o contient du code alors que le .exe contient un programe exécutable

**Prochain exercice : exo4.md**
