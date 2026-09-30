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
    // for-range
    int64_t count = 0;
    for (char c : play)
        if (std::isalpha(c))
            count += 1;
    return count;
}

bool _OrderCharCount(exo5::CharCount a, exo5::CharCount b)
{
    // On doit retourner VRAI si a doit être ordonné avant b
    return a.second > b.second;
}

exo5::CharCountList exo5::GetSortedLetterCount(String const& play)
{ 
    CharCountList res;

    for (char c : play)
    {
        if (std::isalpha(c))
        {
            // Mise en majuscule pour ne pas différencier minuscule/majuscule
            c = std::toupper(c);

            // Est-ce que le caractère a déjà été vu ?
            bool bFound = false;
            for (CharCount& cc : res)
            {
                if (cc.first == c)
                {
                    // Oui: on incrémente le compte
                    cc.second += 1;
                    bFound = true;
                }
            }

            if (!bFound)
            {
                // Non: on rajoute un élément.
                CharCount cc{c, 1};
                res.push_back(cc);
            }
        }
    }
    
#if 1
    std::sort(res.begin(), res.end(), _OrderCharCount);
#endif
    return res;
}

int64_t exo5::CountWords(String const& play)
{
    int64_t count = 0;

    bool bPrevLetter = false;
    for (char c : play)
    {
        if (std::isalpha(c)) // Si 'c' est une lettre
        {
            if (!bPrevLetter) // Si 'c' est précédée par une non-lettre
                count += 1;   // Alors on commence un nouveau mot
            // Sinon, on est déjà à l'intérieur d'un mot
        }
        // Sinon, on est pas dans un mot

        bPrevLetter = std::isalpha(c);
    }
    return count;
}

exo5::StringList exo5::FindWords(String const& play)
{
    exo5::StringList words;

    bool bPrevLetter = false;
    size_t idxWordBegin = 0;
    size_t idxWordEnd = 0;

    for (size_t idx = 0; idx < play.size(); idx += 1)
    {
        char c = play[idx];

        if (std::isalpha(c)) // Si 'c' est une lettre...
        {
            if (!bPrevLetter) // ... précédée par une non-lettre
            {
                idxWordBegin = idx;
            }
        }
        else // Si 'c' est une non-lettre ...
        {
            if (bPrevLetter) // ... précédée par une lettre
            {
                idxWordEnd = idx;

                String word = play.substr(idxWordBegin, idxWordEnd - idxWordBegin);
                words.push_back(word);
            }
        }

        bPrevLetter = std::isalpha(c);
    }

    return words;
}

exo5::StringList exo5::FindUniqueWords(StringList const& words)
{
    return {};
}

exo5::StringCountList exo5::GetSortedWordCount(StringList const& words)
{
    return {};
}