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

> On ne précise plus d'utiliser l'encodage UTF-8.
Les caractères envoyés sous forme de code UTF-8 à la console ne peuvent donc plus etre correctement interprétés.
Seuls les caractères ASCII sont correctement affichés.

C) Dans `exo5_main.cpp`, dans la fonction `GetUserDocumentsFolder()`,
remplacer `#if _WIN64/#else/#endif` par `if(_WIN64)/else`.
Pourquoi le code ne compile pas ?
En déduire une particularité entre `#if` et `if`.
Puis remettre l'état initial.

> En C++, if et else forcent un nouveau scope. Les variables déclarés dans ce scope lui sont locales et ne sont pas accessibles en dehors.
Ici, on essai d'utiliser la variable "path" déclarée dans le scope restreint des conditions en dehors de celles ci. Le compilateur le refuse.
#if n'est pas soumis à cette régle du C++. Il est utilisé par un language propre au préprocesseur qui est lu de maniére linéaire et sans notion de scope.
Aussi, un if standard oblige le compilateur à vérifier chaque condition (meme si elle n'est jamais vraie), contrairement au preprocesseur qui igniore ce qui est conditionné par quelque chose de faux.


D) Dans `exo5_main.cpp`, dans la fonction `main()`, remplacer `#if 0` par `#if 1`.
Relancer le programme. Une section doit apparaître pour chaque pièce de théâtre,
ainsi qu'une section finale ayant des statistiques en moyenne et sur le temps pris.

Dans `exo5_stats.cpp`, le rôle de la fonction `exo5::FindAllPlays()`
est de retrouver toutes les pièces de théâtre présentes dans le fichier
`moliere_integrale.txt` en détectant certaines balises.

E) Replacez les commentaires suivants au bon endroit dans la fonction `exo5::FindAllPlays()`.

F) En utilisant le tableau disponible en-dessous de cette page de documentation
[cette page de documentation](https://en.cppreference.com/cpp/string/byte/isalpha),
trouver la fonction appropriée pour détecter si un caractère est une lettre de l'alphabet latin.

> Cest la fonction std::isalpha .

**IMPORTANT**  
Pour les implémentations de fonction ci-dessous, bien utiliser les types définis dans `exo5_stats.h`
et non pas directement les types standard, ce qui servira plus tard à la fin du TD.

G) Implémenter `exo5::CountLetters()`.
Utiliser la syntaxe `for (char c : play)`.
Comment s'appelle cette syntaxe ?

> C'est une boucle for range.

H) Implémenter `exo5::GetSortedLetterCount()` dans un premier temps sans vous soucier du tri par fréquence.
Quelle fonction standard disponible dans cctype (documentation : [https://en.cppreference.com/cpp/header/cctype](https://en.cppreference.com/cpp/header/cctype))
peut-on utiliser pour ne pas se soucier des différences majuscule/minuscule ?
Vérifier à l'aide du débogueur qu'à la fin du traitement, le tableau `res` ne possède au maximum 26 entrées.

> On peut utiliser std::toupper() .

Dans `exo5::GetSortedLetterCount()`, remplacer `#if 0` par `#if 1`.
La fonction ne compile plus car la fonction `_OrderCharCount()` n'est pas définie.

I) En consultant la documentation de `std::sort()`, et en considérant le type des éléments de `res`,
quel doit être les types d'arguments et de retour (= "la signature") de la fonction `_OrderCharCount()` ?
Que doit indiquer sa valeur de retour ?

([Documentation de std::sort](https://en.cppreference.com/cpp/algorithm/sort))

> La fonction _OrderCharCount() doit prendre en paramétre 2 CharCount à comparer et doit retourner un booléen.
Ce booléen indique à std::sort si le premier élément doit etre placé avant le deuxième (true si oui, false si non).

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