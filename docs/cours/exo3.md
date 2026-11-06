+# Exercice 3

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

> C:\Users\s.denimal\Documents\gIThuB\IntroCppGamedev  / C:\Users\s.denimal\Desktop\IntroCppGameDev

B) Chercher `IntroCppGamedev *.cpp`. Dans quel dossier se trouvent ces fichiers ?

> C:\Users\s.denimal\Documents\gIThuB\IntroCppGamedev  / C:\Users\s.denimal\Desktop\IntroCppGameDev

C) Chercher `IntroCppGamedev *.obj`. Dans quel dossier se trouvent ces fichiers ?
Que remarquez-vous des noms des fichiers concernés ?

> C:\Users\s.denimal\Desktop\IntroCppGameDev\build\obj\x64-windows\Debug\ , ils sont tous dans le dossier Debug

D) Chercher `IntroCppGamedev *.lib`.
Quel(s) fichier(s) apparait(ssent) ?

> JV.lib

Dans la barre de menu de Visual Studio :
- Cliquer sur "Affichage / View" puis "Terminal"
- Entrer la commande `lib /list .\x64-windows\Debug\JV.lib | findstr obj`

E) Que contient le fichier `JV.lib` ?

> le fichier JV.lib contiens plusieurs fichiers obj

F) Chercher `IntroCppGamedev !tools *.exe`. 
Dans quel dossier se trouvent ces fichiers ?

> C:\Users\s.denimal\Desktop\IntroCppGameDev\build\x64-windows\Debug\

Dans Visual Studio, dans la fenêtre "Explorateur de solutions / Solution Explorer" à droite :

- Clic-droit sur "Exo2"
- Cliquer sur "Décharger le projet / Unload project"
- Clic-droit sur "Exo2"
- Cliquer sur "Modifier le fichier projet / Edit project file"

Le fichier ainsi affiché sert de configuration à Visual Studio
pour savoir comment compiler notre base de code C++.

G) Où sont définis les dossiers de sortie des fichiers `.obj` et `.exe` ?

> les fichiers obj sont dans obj\x64-windows\Debug\Exo2\ et les .exe x64-windows\Debug

H) Où est défini comment est résolu les chemins d'include tel que `<jv/jv.h>` ?

> <AdditionalIncludeDirectories>..\src;%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>

I) Chercher `LanguageStandard`. Quel est la version du standard C++ que nous utilisons ?

> stdcpp20 (et on utilise aussi le stdc17)

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

> **Fichiers `.h` et `.cpp` :** .h contiens la déclaration et cpp la défibition du code
>
> **Fichiers `.cpp` et `.obj` :** obj ca contiens le cpp mais en code machine
>
> **Fichiers `.obj` et `.lib` :** rassemble d'autres fichiers obj sur un seul .lib
>
> **Fichiers `.obj` et `.dll` :** rassemble d'autres .obj et stocke les fonctions partagé
>
> **Fichiers `.obj` et `.exe` :** transforme le code machine en code fonctionnel sur une nouvelle fenêtre
>
> **Fichiers `.dll` et `.exe` :** dll dit quoi load et unload a l'ouverture exe

L) Quel est le rôle du préprocesseur ?
Comment reconnait-on les directives de préprocesseur ?

>  Preprocesseur fait le linkage avec les includes et autres et condition par ex #IF.
 On reconnait les directives avec le symbol #

M) Quel est le rôle de l'éditeur de liens ?
Quels sont les deux types de fichiers qu'il peut produire ?
Quelle différence majeure ?

> Après compilation il génère dll exe. Il va assembler les obj en exe/dll 
 exe fait tourner le code mais le dll lui a le code utiliser pas les programs.

**Prochain exercice : exo4.md**
