#include <exo5/exo5_stats.h>

#include <algorithm>

std::vector<exo5::Play> exo5::FindAllPlays(String const& moliere)
{
    std::vector<exo5::Play> plays;

    String remaining{moliere};
    while (true)
    {
        // COMMENTAIRE
        size_t idx = remaining.find("#DEBUT#");
        if (idx == String::npos)
            break;
        exo5::Play& play = plays.emplace_back();

        // COMMENTAIRE
        remaining = remaining.substr(idx + 7);

        // COMMENTAIRE
        idx = remaining.find("#");

        // COMMENTAIRE
        play.name = remaining.substr(0, idx);

        // COMMENTAIRE
        remaining = remaining.substr(idx + 1);

        // COMMENTAIRE
        idx = remaining.find("#");
        play.kind = remaining.substr(0, idx);
        remaining = remaining.substr(idx + 1);

        // COMMENTAIRE
        idx = remaining.find("#FIN#");
        play.content = remaining.substr(0, idx);
        remaining = remaining.substr(idx + 5);

        // COMMENTAIRE
    }

    return plays;
}

int64_t exo5::CountLetters(String const& play)
{
    return 0;
}

exo5::CharCountList exo5::GetSortedLetterCount(String const& play)
{ 
    CharCountList res;
    // TODO
    
#if 0
    std::sort(res.begin(), res.end(), _OrderCharCount);
#endif
    return res;
}

int64_t exo5::CountWords(String const& play)
{
    return 0;
}

exo5::StringList exo5::FindWords(String const& play)
{
    return {};
}

exo5::StringList exo5::FindUniqueWords(StringList const& words)
{
    return {};
}

exo5::StringCountList exo5::GetSortedWordCount(StringList const& words)
{
    return {};
}