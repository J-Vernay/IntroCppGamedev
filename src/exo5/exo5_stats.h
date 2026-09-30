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

using StringList = std::vector<String>;
using CharCount = std::pair<char, int64_t>;
using StringCount = std::pair<String, int64_t>;
using CharCountList = std::vector<CharCount>;
using StringCountList = std::vector<StringCount>;

struct Play
{
    String name;
    String kind; // "COMEDIE", "COMEDIE-BALLET", "COMEDIE HEROIQUE", etc
    String content;
};

bool _OrderCharCount(exo5::CharCount element1, exo5::CharCount element2);

bool _OrderWordCount(exo5::StringCount element1, exo5::StringCount element2);

std::vector<Play> FindAllPlays(String const& moliere);

int64_t CountLetters(String const& play);

CharCountList GetSortedLetterCount(String const& play);

int64_t CountWords(String const& play);

StringList FindWords(String const& play);

StringList FindUniqueWords(StringList const& words);

StringCountList GetSortedWordCount(StringList const& words);

} // namespace exo5