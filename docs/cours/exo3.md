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

> Dossier src

B) Chercher `IntroCppGamedev *.cpp`. Dans quel dossier se trouvent ces fichiers ?

> Dossier src

C) Chercher `IntroCppGamedev *.obj`. Dans quel dossier se trouvent ces fichiers ?
Que remarquez-vous des noms des fichiers concernés ?

> C:\Users\q.piette\source\repos\IntroCppGamedev\build\obj\x64-windows\Debug 
relié a des .cpp

D) Chercher `IntroCppGamedev *.lib`.
Quel(s) fichier(s) apparait(ssent) ?

> Le fichier qui apparait est le fichier JV.lib

Dans la barre de menu de Visual Studio :
- Cliquer sur "Affichage / View" puis "Terminal"
- Entrer la commande `lib /list .\x64-windows\Debug\JV.lib | findstr obj`

E) Que contient le fichier `JV.lib` ?

>  Il contient tous les fichiers jv obj qui correspondent a des fichiers cpp du projet JV

F) Chercher `IntroCppGamedev !tools *.exe`. 
Dans quel dossier se trouvent ces fichiers ?

> dans build x-64 windows Debug

Dans Visual Studio, dans la fenêtre "Explorateur de solutions / Solution Explorer" à droite :

- Clic-droit sur "Exo2"
- Cliquer sur "Décharger le projet / Unload project"
- Clic-droit sur "Exo2"
- Cliquer sur "Modifier le fichier projet / Edit project file"

Le fichier ainsi affiché sert de configuration à Visual Studio
pour savoir comment compiler notre base de code C++.

G) Où sont définis les dossiers de sortie des fichiers `.obj` et `.exe` ?

>    <OutDir>$(ProjectDir)x64-windows\Debug\</OutDir>
    <IntDir>$(ProjectDir)obj\x64-windows\Debug\Exo2\</IntDir>

H) Où est défini comment est résolu les chemins d'include tel que `<jv/jv.h>` ?

> ou le compilateur cherche les fichiers includes 
 <AdditionalIncludeDirectories>..\src;%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>

I) Chercher `LanguageStandard`. Quel est la version du standard C++ que nous utilisons ?

> stdcpp20  <LanguageStandard>stdcpp20</LanguageStandard>

J) Où sont définis quels fichiers C++ sont compilés pour l'exercice 2 ?

> 

Dans Visual Studio, dans la fenêtre "Explorateur de solutions" à droite :

- Clic-droit sur "Exo2"
- Cliquer sur "Recharger le projet / Reload project"

K) Quels sont les liens entre :

> **Fichiers `.h` et `.cpp` :** include 
>
> **Fichiers `.cpp` et `.obj` :** compilation
>
> **Fichiers `.obj` et `.lib` :** archive 
>
> **Fichiers `.obj` et `.dll` :** edition des liens
>
> **Fichiers `.obj` et `.exe` :** edition des liens
>
> **Fichiers `.dll` et `.exe` :** appeles fonctions avec reso dynamique

L) Quel est le rôle du préprocesseur ?
Comment reconnait-on les directives de préprocesseur ?

> il intervient avant toute notion de c++ il fonctionne avec les diez c'est comme ça qu'on reconnait les directives


M) Quel est le rôle de l'éditeur de liens ?
Quels sont les deux types de fichiers qu'il peut produire ?
Quelle différence majeure ?

> ... la resolution de liens avec une dll est faite a l'exe alors qu'une lib -> a la compilation
Lib = Archive = Librairie statique
Dll = Librairie Dynamique

interet dll : 
mettre a jour les fichiers exe et dll independamment
peut partager du code entre plusieurs exe 

permet d'eco espace disque

**Prochain exercice : exo4.md**
