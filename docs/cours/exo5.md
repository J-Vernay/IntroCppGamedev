# Exercice 5

**OBJECTIF**  
- Manipuler du texte en C++
- Voir un programme "C++" classique (sans moteur).

**RESSOURCES**  
- [std::string](https://en.cppreference.com/cpp/string/basic_string)
- [std::string_view](https://en.cppreference.com/cpp/string/basic_string_view)
- [Containers](https://en.cppreference.com/cpp/container)
- [Character types](https://en.cppreference.com/cpp/header/cctype)
- [Algorithms](https://en.cppreference.com/cpp/algorithm)

## TD

Le but de ce TD est de lire un fichier texte contenant des pièces de théâtre de Molière
et d'en retirer des statistiques.

Dans l'explorateur de solutions, à droite, clic-droit sur "Exo 5",
puis clic sur "Définir en tant que projet de démarrage / Set as Startup Project".

A) Prendre connaissance des fichiers fournis et exécutez le programme.
Vous devriez avoir un affichage texte qui apparaît ("la console")
contenant un extrait d'une pièce de Molière.

B) Dans `exo5_main.cpp`, au début de la fonction `main()`,
enlever la ligne `std::setlocale(LC_ALL, ".UTF8");`.
Quel effet cela produit sur la sortie texte dans la console ?
Puis remettre la ligne.

> ...

C) Dans `exo5_main.cpp`, dans la fonction `GetUserDocumentsFolder()`,
remplacer `#if _WIN64/#else/#endif` par `if(_WIN64)/else`.
Pourquoi le code ne compile pas ?
En déduire une particularité entre `#if` et `if`.
Puis remettre l'état initial.

> ...

D) Dans `exo5_main.cpp`, dans la fonction `main()`, remplacer `#if 0` par `#if 1`.
Relancer le programme. Une section doit apparaître pour chaque pièce de théâtre,
ainsi qu'une section finale ayant des statistiques en moyenne et sur le temps pris.

Dans `exo5_stats.cpp`, le rôle de la fonction `exo5::FindAllPlays()`
est de retrouver toutes les pièces de théâtre présentes dans le fichier
`moliere_integrale.txt` en détectant certaines balises.

E) Replacez les commentaires suivants au bon endroit dans la fonction `exo5::FindAllPlays()`.
 
> // On récupère le nom du titre de la pièce.
> 
> // Il y a le contenu de la pièce, jusqu'à "#FIN#"
> 
> // Le début de remaining correspond au titre de la pièce.
> // On cherche le prochain '#', qui indique la fin du titre.
> 
> // Retour au début, où l'on cherche le prochain #DEBUT#
> 
> // On retire le titre de la pièce et le "#" d'après.
> 
> // On enlève tout ce qui est avant "#DEBUT#" (inclus)
> 
> // On cherche la position du premier caractère de la prochaine occurrence de "#DEBUT#"
> 
> // Le début de remaining correspond au type de pièce (ex: COMEDIE).

F) En utilisant le tableau disponible en-dessous de cette page de documentation
[cette page de documentation](https://en.cppreference.com/cpp/string/byte/isalpha),
trouver la fonction appropriée pour détecter si un caractère est une lettre de l'alphabet latin.


**IMPORTANT**  
Pour les implémentations de fonction ci-dessous, bien utiliser les types définis dans `exo5_stats.h`
et non pas directement les types standard, ce qui servira plus tard à la fin du TD.

G) Implémenter `exo5::CountLetters()`.
Utiliser la syntaxe `for (char c : play)`.
Comment s'appelle cette syntaxe ?

> ...

H) Implémenter `exo5::GetSortedLetterCount()` dans un premier temps sans vous soucier du tri par fréquence.
Quelle fonction standard disponible dans cctype (documentation : [https://en.cppreference.com/cpp/header/cctype](https://en.cppreference.com/cpp/header/cctype))
peut-on utiliser pour ne pas se soucier des différences majuscule/minuscule ?
Vérifier à l'aide du débogueur qu'à la fin du traitement, le tableau `res` ne possède au maximum 26 entrées.

> ...

Dans `exo5::GetSortedLetterCount()`, remplacer `#if 0` par `#if 1`.
La fonction ne compile plus car la fonction `_OrderCharCount()` n'est pas définie.

I) En consultant la documentation de `std::sort()`, et en considérant le type des éléments de `res`,
quel doit être les types d'arguments et de retour (= "la signature") de la fonction `_OrderCharCount()` ?
Que doit indiquer sa valeur de retour ?

([Documentation de std::sort](https://en.cppreference.com/cpp/algorithm/sort))

> ...

J) Définir et implémenter la fonction `_OrderCharCount()`.
Exécuter le programme.
Le résultat devrait être :

        ========== LES AMANS MAGNIFIQUES ==========
        ...
        Top 10 lettres :
        1. E (9810)
        2. S (5733)
        3. I (4660)
        4. R (4360)
        5. T (4307)
        6. U (4184)
        7. A (4108)
        8. N (4000)
        9. O (3962)
        10. L (3058)

K) Implémenter `exo5::CountWords()`. Un mot est défini comme un groupe de lettres
séparé de part et d'autre par un caractère non-lettre.

L) Implémenter `exo5::FindWords()`.

M) Implémenter `exo5::FindUniqueWords()`

N) Implémenter `exo5::GetSortedWordCount()`

Vous devriez obtenir :

        ========== LES AMANS MAGNIFIQUES ==========
        ...
        Nb mots         : 15635
        Nb mots uniques : 2687
        Top 10 mots :
        1. de (576)
        2. vous (342)
        3. et (342)
        4. que (312)
        5. la (226)
        6. le (216)
        7. je (210)
        8. l (201)
        9. d (196)
        10. les (196)

        
**Prochain exercice : exo5_analyse.md**