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

> dans C:\Users\a.delattre\IntroCppGameDev\src

B) Chercher `IntroCppGamedev *.cpp`. Dans quel dossier se trouvent ces fichiers ?

> meme choses?

C) Chercher `IntroCppGamedev *.obj`. Dans quel dossier se trouvent ces fichiers ?
Que remarquez-vous des noms des fichiers concernés ?

> le nom des fichiers vient des .cpp du projet

D) Chercher `IntroCppGamedev *.lib`.
Quel(s) fichier(s) apparait(ssent) ?

> le fichier JV.Lib apparait

Dans la barre de menu de Visual Studio :
- Cliquer sur "Affichage / View" puis "Terminal"
- Entrer la commande `lib /list .\x64-windows\Debug\JV.lib | findstr obj`

E) Que contient le fichier `JV.lib` ?

> il contient d'autre fichier .obj

F) Chercher `IntroCppGamedev !tools *.exe`. 
Dans quel dossier se trouvent ces fichiers ?

> Dans le dossier debug

Dans Visual Studio, dans la fenêtre "Explorateur de solutions / Solution Explorer" à droite :

- Clic-droit sur "Exo2"
- Cliquer sur "Décharger le projet / Unload project"
- Clic-droit sur "Exo2"
- Cliquer sur "Modifier le fichier projet / Edit project file"

Le fichier ainsi affiché sert de configuration à Visual Studio
pour savoir comment compiler notre base de code C++.

G) Où sont définis les dossiers de sortie des fichiers `.obj` et `.exe` ?

> dans OutDir et IntDir

H) Où est défini comment est résolu les chemins d'include tel que `<jv/jv.h>` ?

> c'est dans AdditionalIncludeDirectories

I) Chercher `LanguageStandard`. Quel est la version du standard C++ que nous utilisons ?

> c'est du cpp 20 

J) Où sont définis quels fichiers C++ sont compilés pour l'exercice 2 ?

> dans: 
 <ProjectReference>
    </ProjectReference>
  </ItemDefinitionGroup>
  <ItemGroup>
    <ClInclude Include="..\src\exo2\exo2_draw.h" />
    <ClInclude Include="..\src\exo2\exo2_pong.h" />
  </ItemGroup>
  <ItemGroup>
    <ClCompile Include="..\src\exo2\exo2_draw.cpp" />
    <ClCompile Include="..\src\exo2\exo2_main.cpp" />
    <ClCompile Include="..\src\exo2\exo2_pong.cpp" />
  </ItemGroup>
  <ItemGroup>
    <ProjectReference Include="JV.vcxproj">

Dans Visual Studio, dans la fenêtre "Explorateur de solutions" à droite :

- Clic-droit sur "Exo2"
- Cliquer sur "Recharger le projet / Reload project"

K) Quels sont les liens entre :

> **Fichiers `.h` et `.cpp` :** cpp inclut .h
>
> **Fichiers `.cpp` et `.obj` :** .cpp compilet en .obj
>
> **Fichiers `.obj` et `.lib` :** .obj sont rassembler pour creer une .lib
>
> **Fichiers `.obj` et `.dll` :** les .obj sont lier par des lbikers pour crrer un .dll
>
> **Fichiers `.obj` et `.exe` :** les .obj sont lier pour creer des executable .exe
>
> **Fichiers `.dll` et `.exe` :** le .exe utilise et charge le code du .dll pendant son execution
L) Quel est le rôle du préprocesseur ?
Comment reconnait-on les directives de préprocesseur ?

> il prepare le code source avant quil sois mis au compilateur, souvent reconnu avec le #

M) Quel est le rôle de l'éditeur de liens ?
Quels sont les deux types de fichiers qu'il peut produire ?
Quelle différence majeure ?

> son role est dassembler les bibliotheque pour creer un .exe, il produit des .exe ou des bibliotheque (.dll ou .lib), la difference majeur est que les .exe ont une fonction main qui est appeler par l'os mais pas les .dll, eux sont appeler par le main/

**Prochain exercice : exo4.md**
