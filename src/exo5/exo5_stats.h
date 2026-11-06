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

std::vector<Play> FindAllPlays(String const& moliere);

int64_t CountLetters(String const& play);

CharCountList GetSortedLetterCount(String const& play);

int64_t CountWords(String const& play);

StringList FindWords(String const& play);

StringList FindUniqueWords(StringList const& words);

StringCountList GetSortedWordCount(StringList const& words);

bool _OrderCharCount(
    const std::pair<char, int64_t> firstElement, const std::pair<char, int64_t> secondElement);

} // namespace exo5