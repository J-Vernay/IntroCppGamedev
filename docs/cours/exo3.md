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
IntroCppGamedev/src
> ...

B) Chercher `IntroCppGamedev *.cpp`. Dans quel dossier se trouvent ces fichiers ?
IntroCppGamedev/src
> ...

C) Chercher `IntroCppGamedev *.obj`. Dans quel dossier se trouvent ces fichiers ?
Que remarquez-vous des noms des fichiers concernés ?
IntroCppGamedev\build\obj\x64-windows\Debug, et les dossier porte le meme nom que leur extantion .cpp
> ...

D) Chercher `IntroCppGamedev *.lib`.
Quel(s) fichier(s) apparait(ssent) ?
un fichier JV.lib
> ...

Dans la barre de menu de Visual Studio :
- Cliquer sur "Affichage / View" puis "Terminal"
- Entrer la commande `lib /list .\x64-windows\Debug\JV.lib | findstr obj`

E) Que contient le fichier `JV.lib` ?
IntroCppGamedev\build\obj\x64-windows\Debug\JV\jv_random.obj         
IntroCppGamedev\build\obj\x64-windows\Debug\JV\jv_win64_file.obj     
IntroCppGamedev\build\obj\x64-windows\Debug\JV\jv_win64_main.obj     
IntroCppGamedev\build\obj\x64-windows\Debug\JV\jv_win64_renderer.obj 
IntroCppGamedev\build\obj\x64-windows\Debug\JV\jv_win64_shaders.obj  
IntroCppGamedev\build\obj\x64-windows\Debug\JV\jv_win64_window.obj   

os\obj\amd64fre\onecoreuap\windows\directx\misc\dxguid\daytona\objfre\amd64\d3d10guid.ob
os\obj\amd64fre\onecoreuap\windows\directx\misc\dxguid\daytona\objfre\amd64\d3d9guid.obj
os\obj\amd64fre\onecoreuap\windows\directx\misc\dxguid\daytona\objfre\amd64\dxguid.obj  


> ...

F) Chercher `IntroCppGamedev !tools *.exe`. 
Dans quel dossier se trouvent ces fichiers ?
IntroCppGamedev\build\x64-windows\Debug

> ...

Dans Visual Studio, dans la fenêtre "Explorateur de solutions / Solution Explorer" à droite :

- Clic-droit sur "Exo2"
- Cliquer sur "Décharger le projet / Unload project"
- Clic-droit sur "Exo2"
- Cliquer sur "Modifier le fichier projet / Edit project file"

Le fichier ainsi affiché sert de configuration à Visual Studio
pour savoir comment compiler notre base de code C++.

G) Où sont définis les dossiers de sortie des fichiers `.obj` et `.exe` ?
 A 
    <OutDir>$(ProjectDir)x64-windows\Debug\</OutDir>
    <IntDir>$(ProjectDir)obj\x64-windows\Debug\Exo2\</IntDir>
> ...

H) Où est défini comment est résolu les chemins d'include tel que `<jv/jv.h>` ?
 A 

 A <ClInclude Include="..\src\exo2\exo2_draw.h" />

> ...

I) Chercher `LanguageStandard`. Quel est la version du standard C++ que nous utilisons ?
 I 
       <LanguageStandard>stdcpp20</LanguageStandard> donc cpp c++20
     

> ...

J) Où sont définis quels fichiers C++ sont compilés pour l'exercice 2 ?
 A 
     <ClCompile Include="..\src\exo2\exo2_main.cpp" />

> ...

Dans Visual Studio, dans la fenêtre "Explorateur de solutions" à droite :

- Clic-droit sur "Exo2"
- Cliquer sur "Recharger le projet / Reload project"

K) Quels sont les liens entre :

> **Fichiers `.h` et `.cpp` :** ...declaction partager pour les .h, et implémentation avec include pour les cpp
>
> **Fichiers `.cpp` et `.obj` :** ...un cpp produit un .obj a la compilation, contenant du language machine
>
> **Fichiers `.obj` et `.lib` :** ....lib regroupe les fichier .obj
>
> **Fichiers `.obj` et `.dll` :** ... l'editeur de liens assemble les .obj en lib dynamique
>
> **Fichiers `.obj` et `.exe` :** ... le .exe est constiuer de tout les .obj et les bibliotheque nécessaire a sa création
>
> **Fichiers `.dll` et `.exe` :** ... l'editeur de liens peux produire en sortie un .dll ou .exe

L) Quel est le rôle du préprocesseur ?
Comment reconnait-on les directives de préprocesseur ?
son role est de preparer le code avant sa compilation, il insere mes macros et indication heritant du C (comme #include, #define #if...)

> ...

M) Quel est le rôle de l'éditeur de liens ?
Quels sont les deux types de fichiers qu'il peut produire ?
Quelle différence majeure ?

l'editeur de liens assemble le code contenue dans les .obj et ses lib, un editeur de liens peux produire en sortie un .dll ou .exe, un .exe est un exécutable, un .dll dois etre utiliser par un autre programme, et ne peux pas etre utiliser tout seul

> ...

**Prochain exercice : exo4.md**
