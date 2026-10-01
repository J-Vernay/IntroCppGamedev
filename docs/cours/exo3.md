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

> Les fichiers se trouve dans scr

B) Chercher `IntroCppGamedev *.cpp`. Dans quel dossier se trouvent ces fichiers ?

> Les fichiers se trouve dans scr

C) Chercher `IntroCppGamedev *.obj`. Dans quel dossier se trouvent ces fichiers ?
Que remarquez-vous des noms des fichiers concernés ?

> Dans build/obj , se sont des fichier que nous avons deja compilé

D) Chercher `IntroCppGamedev *.lib`.
Quel(s) fichier(s) apparait(ssent) ?

> JV.lib

Dans la barre de menu de Visual Studio :
- Cliquer sur "Affichage / View" puis "Terminal"
- Entrer la commande `lib /list .\x64-windows\Debug\JV.lib | findstr obj`

E) Que contient le fichier `JV.lib` ?

> Le fichier contient les .obj de jv et de windows

F) Chercher `IntroCppGamedev !tools *.exe`. 
Dans quel dossier se trouvent ces fichiers ?

> Dans le dossier repos\IntroCppGamedev\debug

Dans Visual Studio, dans la fenêtre "Explorateur de solutions / Solution Explorer" à droite :

- Clic-droit sur "Exo2"
- Cliquer sur "Décharger le projet / Unload project"
- Clic-droit sur "Exo2"
- Cliquer sur "Modifier le fichier projet / Edit project file"

Le fichier ainsi affiché sert de configuration à Visual Studio
pour savoir comment compiler notre base de code C++.

G) Où sont définis les dossiers de sortie des fichiers `.obj` et `.exe` ?

> '<OutDir>' '<IntDir>'

H) Où est défini comment est résolu les chemins d'include tel que `<jv/jv.h>` ?

> la balise <AdditionalIncludeDirectories> sous la section <ClCompile>

I) Chercher `LanguageStandard`. Quel est la version du standard C++ que nous utilisons ?

> stdcpp20 donc c++20

J) Où sont définis quels fichiers C++ sont compilés pour l'exercice 2 ?

> ils sont définis dans les blocs <ItemGroup> situés vers la fin du fichier

Dans Visual Studio, dans la fenêtre "Explorateur de solutions" à droite :

- Clic-droit sur "Exo2"
- Cliquer sur "Recharger le projet / Reload project"

K) Quels sont les liens entre :

> **Fichiers `.h` et `.cpp` :** Les fichiers .h contiennent les déclarations. Ils sont inclus dans les fichiers .cpp qui contiennent les définitions/implémentations
>
> **Fichiers `.cpp` et `.obj` :** Chaque fichier .cpp est compilé individuellement par le compilateur pour produire un fichier objet .obj contenant du code machine.
>
> **Fichiers `.obj` et `.lib` :** Plusieurs fichiers .obj peuvent être regroupés par un éditeur de liens pour créer une bibliothèque statique . Les .lib peuvent aussi être des "import libraries" associées aux .dll.
>
> **Fichiers `.obj` et `.dll` :** L'éditeur de liens peut assembler plusieurs fichiers .obj pour créer une bibliothèque dynamique , dont le code sera chargé au moment de l'exécution.
>
> **Fichiers `.obj` et `.exe` :** L'éditeur de liens rassemble tous les fichiers .obj du projet pour générer l'exécutable final .exe.
>
> **Fichiers `.dll` et `.exe` :** Un fichier .exe utilise des fonctions définies dans un fichier .dll. Le .dll doit généralement se trouver dans le même dossier que le .exe pour que le programme puisse démarrer et s'exécuter.

L) Quel est le rôle du préprocesseur ?
Comment reconnait-on les directives de préprocesseur ?

> ...

M) Quel est le rôle de l'éditeur de liens ?
Quels sont les deux types de fichiers qu'il peut produire ?
Quelle différence majeure ?

> L'éditeur de liens prend tous les fichiers objets générés par le compilateur ainsi que les bibliothèques, résout les références croisées et assemble le tout en un fichier binaire final.
'.exe' et '.dll'

Un fichier .exe possède un point d'entrée principal et peut être exécuté de façon autonome par l'utilisateur/système d'exploitation.

Une .dll ne possède pas de point d'entrée exécutable autonome, elle contient du code réutilisable destiné à être chargé et appelé par un autre programme.

**Prochain exercice : exo4.md**
