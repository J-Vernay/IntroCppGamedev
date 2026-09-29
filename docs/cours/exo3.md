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

> Le dossier "src".

B) Chercher `IntroCppGamedev *.cpp`. Dans quel dossier se trouvent ces fichiers ?

> Le dossier "src".

C) Chercher `IntroCppGamedev *.obj`. Dans quel dossier se trouvent ces fichiers ?
Que remarquez-vous des noms des fichiers concernés ?

> IntroCppGamedev/build/obj/x64-windows/Debug
Ce sont les noms de tout les fichiers .cpp du projet. Seul l'extension change pour .obj .

D) Chercher `IntroCppGamedev *.lib`.
Quel(s) fichier(s) apparait(ssent) ?

> JV.lib

Dans la barre de menu de Visual Studio :
- Cliquer sur "Affichage / View" puis "Terminal"
- Entrer la commande `lib /list .\x64-windows\Debug\JV.lib | findstr obj`

E) Que contient le fichier `JV.lib` ?

> Il contient les versions compilés des fichier d'implémentation du moteur de jeu.

F) Chercher `IntroCppGamedev !tools *.exe`. 
Dans quel dossier se trouvent ces fichiers ?

> IntroCppGamedev/build/x64-windows/Debug

Dans Visual Studio, dans la fenêtre "Explorateur de solutions / Solution Explorer" à droite :

- Clic-droit sur "Exo2"
- Cliquer sur "Décharger le projet / Unload project"
- Clic-droit sur "Exo2"
- Cliquer sur "Modifier le fichier projet / Edit project file"

Le fichier ainsi affiché sert de configuration à Visual Studio
pour savoir comment compiler notre base de code C++.

G) Où sont définis les dossiers de sortie des fichiers `.obj` et `.exe` ?

> Les dossiers de sortie des fichiers .obj et .exe sont définis respectivement par les balises <IntDir> et <OutDir> pour chaque mode de compilation.

H) Où est défini comment est résolu les chemins d'include tel que `<jv/jv.h>` ?

> Cela est défini par les balises <AdditionalIncludeDirectories> pour chaque mode de compilation.

I) Chercher `LanguageStandard`. Quel est la version du standard C++ que nous utilisons ?

> La version du standard C++ utilisée est C++20 (stdcpp20).

J) Où sont définis quels fichiers C++ sont compilés pour l'exercice 2 ?

> Ils sont définis par les balises <ClCompile Include="chemin d'accès" />

Dans Visual Studio, dans la fenêtre "Explorateur de solutions" à droite :

- Clic-droit sur "Exo2"
- Cliquer sur "Recharger le projet / Reload project"

K) Quels sont les liens entre :

> **Fichiers `.h` et `.cpp` :
Le .h déclare un contrat que les .cpp doivent suivre s'ils l'incluent.

> **Fichiers `.cpp` et `.obj` : 
Les .cpp sont compilés individuellement en .obj par le compilateur.

> **Fichiers `.obj` et `.lib` :
Les bibliotheques statiques (.lib) regroupent plusieurs .obj pour compiler tout un module seulement une fois et le fournir aux parties du projet le demandant.

> **Fichiers `.obj` et `.dll` : 
Le Linker peut assembler plusieurs fichiers .obj en une bibliotheque dynamique (fichier .dll)

> **Fichiers `.obj` et `.exe` :
Le Linker peut assembler plusieurs fichiers .obj en un executable Windows (.exe), si cet ensemble de fichier contient un point d'entrée d'application Windows (WinMain).

> **Fichiers `.dll` et `.exe` :
Un executable Windows peut faire appel au code d'une bibliothèque .dll sans l'intégrer dans son propre code binaire.
L'OS charge les fichiers .dll requis en RAM et relie dynamiquement leurs fonctions à l'executable.

L) Quel est le rôle du préprocesseur ?
Comment reconnait-on les directives de préprocesseur ?

> Le préprocesseur intervient avant la compilation. Il effectue des modifications textuelles sur le fichier source en le traitant comme du texte et non comme du code C++.
Il ne comprend que ses propres directives caractérisées par le symbole # .

M) Quel est le rôle de l'éditeur de liens ?
Quels sont les deux types de fichiers qu'il peut produire ?
Quelle différence majeure ?

> L'éditeur de liens (Linker) arrive après l'étape de compilation et sert à resoudre les références puis assembler les fichiers .obj en un seul fichier.
Ce fichier peut etre un executable ou une bibliotheque dynamique (.dll).
Le choix du type de fichier dépend de si les objets à assembler contiennent un point d'entrée autonome.
Si c'est le cas, on obtient un .exe pouvant etre executé directement.
Sinon, on obtient un .dll qui ne sert qu'à etre appelé par des .exe .

**Prochain exercice : exo4.md**
