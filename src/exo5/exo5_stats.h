#pragma once

#include <deque>
#include <list>
#include <map>
#include <set>
#include <stdint.h>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace exo5
{

using String = std::string_view;

using StringList = std::list<String>;
using CharCount = std::pair<char, int64_t>;
using StringCount = std::pair<String, int64_t>;
using CharCountList = std::list<CharCount>;
using StringCountList = std::list<StringCount>;

struct Play
{
    String name;
    String kind; // "COMEDIE", "COMEDIE-BALLET", "COMEDIE HEROIQUE", etc
    String content;
};

std::vector<Play> FindAllPlays(String const& moliere);

/// Retourne le nombre de lettres (A-Z a-z) dans "play"
int64_t CountLetters(String const& play);

/// Retourne une liste de (char, int) qui donne le nombre
/// d'occurrence de chaque lettre dans 'play'.
/// Le résultat doit être trié de la lettre la plus fréquente à la lettre la moins fréquente.
CharCountList GetSortedLetterCount(String const& play);

/// Retourne le nombre de mots (= un groupe de lettres A-Z a-z) dans "play".
int64_t CountWords(String const& play);

/// Retourne la liste de tous les mots (= groupes de lettres) dans "play".
StringList FindWords(String const& play);

/// Crée une nouvelle liste contenant les mots de "words" une seule fois chacun.
/// (la liste "words" provient de ce qui est retourné par FindWords())
StringList FindUniqueWords(StringList const& words);

/// Retourne une liste de (String, int) qui donne le nombre d'occurrence
/// de chaque mot dans la liste "words"
/// Le résultat doit être trié du mot le plus fréquent au mot le moins fréquent.
/// (la liste "words" provient de ce qui est retourné par FindWords())
StringCountList GetSortedWordCount(StringList const& words);

} // namespace exo5