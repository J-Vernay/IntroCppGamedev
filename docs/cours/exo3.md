# Exercice 3

**OBJECTIF**  
- Comprendre le découpage en plusieurs fichiers en C++
- Comprendre la compilation en C++

**RESSOURCES**
- [Cours sur la compilation en C++)](https://laefy.github.io/CPP_Learning/chapter1/5-compilation/)

## TD

Ouvrir le logiciel "Everything" dans le dossier `tools` du dépôt de code.

Dans la barre de menu en haut, dans "Recherche",
vérifier que "Respecter le chemin" est coché.

A) Chercher `IntroCppGamedev *.h`. Dans quel dossier se trouvent ces fichiers ?

> ...

B) Chercher `IntroCppGamedev *.cpp`. Dans quel dossier se trouvent ces fichiers ?

> ...

C) Chercher `IntroCppGamedev *.obj`. Dans quel dossier se trouvent ces fichiers ?
Que remarquez-vous des noms des fichiers concernés ?

> ...

D) Chercher `IntroCppGamedev *.lib`.
Quel(s) fichier(s) apparait(ssent) ?

> ...

Dans la barre de menu de Visual Studio :
- Cliquer sur "Affichage" puis "Terminal"
- Entrer la commande `lib /list .\x64-windows\Debug\JV.lib | findstr obj`

E) Que contient le fichier `JV.lib` ?

> ...

F) Chercher `IntroCppGamedev !tools *.exe`. 
Dans quel dossier se trouvent ces fichiers ?

> ...

Dans Visual Studio, dans la fenêtre "Explorateur de solutions" à droite :

- Clic-droit sur "Exo2"
- Cliquer sur "Décharger le projet"
- Clic-droit sur "Exo2"
- Cliquer sur "Modifier le fichier projet"

Le fichier ainsi affiché sert de configuration à Visual Studio
pour savoir comment compiler notre base de code C++.

G) Où sont définis les dossiers de sortie des fichiers `.obj` et `.exe` ?

> ...

H) Où est défini comment est résolu les chemins d'include tel que `<jv/jv.h>` ?

> ...

I) Chercher `LanguageStandard`. Quel est la version du standard C++ que nous utilisons ?

> ...

J) Où sont définis quels fichiers C++ sont compilés pour l'exercice 2 ?

> ...

Dans Visual Studio, dans la fenêtre "Explorateur de solutions" à droite :

- Clic-droit sur "Exo2"
- Cliquer sur "Recharger le projet"

K) Quels sont les liens entre :

> **Fichiers `.h` et `.cpp` :** ...
>
> **Fichiers `.cpp` et `.obj` :** ...
>
> **Fichiers `.obj` et `.lib` :** ...
>
> **Fichiers `.obj` et `.dll` :** ...
>
> **Fichiers `.obj` et `.exe` :** ...
>
> **Fichiers `.dll` et `.exe` :** ...

L) Quel est le rôle du préprocesseur ?
Comment reconnait-on les directives de préprocesseur ?

> ...

M) Quel est le rôle de l'éditeur de liens ?
Quels sont les deux types de fichiers qu'il peut produire ?
Quelle différence majeure ?

> ...

**Prochain exercice : exo4.md**
