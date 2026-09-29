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
> Il reprennent le nom de fichier cpp et correspondent seulement au projet que l'on a déjà build

D) Chercher `IntroCppGamedev *.lib`.
Quel(s) fichier(s) apparait(ssent) ?

> JV.lib

Dans la barre de menu de Visual Studio :
- Cliquer sur "Affichage / View" puis "Terminal"
- Entrer la commande `lib /list .\x64-windows\Debug\JV.lib | findstr obj`

E) Que contient le fichier `JV.lib` ?

> Une liste des fichiers .obj du projet/dossier JV

F) Chercher `IntroCppGamedev !tools *.exe`. 
Dans quel dossier se trouvent ces fichiers ?

> IntroCppGamedev\build\x64-windows\Debug

Dans Visual Studio, dans la fenêtre "Explorateur de solutions / Solution Explorer" à droite :

- Clic-droit sur "Exo2"
- Cliquer sur "Décharger le projet / Unload project"
- Clic-droit sur "Exo2"
- Cliquer sur "Modifier le fichier projet / Edit project file"

Le fichier ainsi affiché sert de configuration à Visual Studio
pour savoir comment compiler notre base de code C++.

G) Où sont définis les dossiers de sortie des fichiers `.obj` et `.exe` ?

 > 
    <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug x64-windows|x64'">
        <LinkIncremental>false</LinkIncremental>
        <OutDir>$(ProjectDir)x64-windows\Debug\</OutDir>
        <IntDir>$(ProjectDir)obj\x64-windows\Debug\Exo2\</IntDir>
        <TargetName>Exo2</TargetName>
        <TargetExt>.exe</TargetExt>
    </PropertyGroup>

H) Où est défini comment est résolu les chemins d'include tel que `<jv/jv.h>` ?

>
    <ItemGroup>
        <ProjectReference Include="JV.vcxproj">
            <Project>{25745900-1100-880B-7AAE-880B6659880B}</Project>
        </ProjectReference>
    </ItemGroup>

I) Chercher `LanguageStandard`. Quel est la version du standard C++ que nous utilisons ?

> "stdcpp20", ou C++ 20.

J) Où sont définis quels fichiers C++ sont compilés pour l'exercice 2 ?

> 
    <ItemGroup>
        <ClInclude Include="..\src\exo2\exo2_draw.h" />
        <ClInclude Include="..\src\exo2\exo2_pong.h" />
    </ItemGroup>
    <ItemGroup>
        <ClCompile Include="..\src\exo2\exo2_draw.cpp" />
        <ClCompile Include="..\src\exo2\exo2_main.cpp" />
        <ClCompile Include="..\src\exo2\exo2_pong.cpp" />
    </ItemGroup>

Dans Visual Studio, dans la fenêtre "Explorateur de solutions" à droite :

- Clic-droit sur "Exo2"
- Cliquer sur "Recharger le projet / Reload project"

K) Quels sont les liens entre :

> **Fichiers `.h` et `.cpp`:** Les fichiers .h sont des headers qui définissent quels méthode et attributs existes et leurs encapsulement
>
> **Fichiers `.cpp` et `.obj`:** Les fichier cpp produisent un fichier obj lors de la compilation
>
> **Fichiers `.obj` et `.lib`:** Les fichier .lib sont une combinaison en un seul fichier de fichiers .obj.
>
> **Fichiers `.obj` et `.dll`:** Les fichier .dll sont une combinaison en un seul fichier de fichiers .obj qui peuvent être partagé (une seul copie) vers d'autre projets.
>
> **Fichiers `.obj` et `.exe`:** Les fichiers .exe sont une combinaison en un seul fichier éxécutable (avec Main ou WinMain en point d'entrée) de fichiers .obj.
>
> **Fichiers `.dll` et `.exe`:** Les fichiers .exe peuvent faire appellent à des bibliothèque communes présentes dans des .dll

L) Quel est le rôle du préprocesseur ?
Comment reconnait-on les directives de préprocesseur ?

> Il intervient avant la compilation et permet notamment l'inclusion du code source provenant d'autre fichier et de préparer le fichier en fonction de la compilation conditionnelle
> Les directives de préprocesseur commencent par "#" (#include, #define, #if...)

M) Quel est le rôle de l'éditeur de liens ?
Quels sont les deux types de fichiers qu'il peut produire ?
Quelle différence majeure ?

> L'éditeur de lien intervient après la compilation du code, il crée un fichier éxécutable ou une bilbiothèque à partir des fichiers .obj.
>
> .exe, .lib ou .dll
>
> .exe est directement éxécutable, 
> .lib permet de réutiliser du code compilé dans plusieurs programme et est chargé pour chaque programmes l'utilisant,
> .dll est similaire au .lib mais n'est chargé qu'une seul fois puis réutilisé par les programmes pour réduire la mémoire consommée. 

**Prochain exercice : exo4.md**
