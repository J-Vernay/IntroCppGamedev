#include <exo5/exo5_stats.h>

#include <algorithm>

std::vector<exo5::Play> exo5::FindAllPlays(String const& moliere)
{
    std::vector<exo5::Play> plays;

    String remaining{moliere};
    while (true)
    {
        // On cherche la position du premier caractère de la prochaine occurrence de "#DEBUT#"
        size_t idx = remaining.find("#DEBUT#");
        if (idx == String::npos)
            break;
        exo5::Play& play = plays.emplace_back();

        // On enlève tout ce qui est avant "#DEBUT#" (inclus)
        remaining = remaining.substr(idx + 7);

        // Le début de remaining correspond au titre de la pièce.
        // On cherche le prochain '#', qui indique la fin du titre.
        idx = remaining.find("#");

        // On récupère le nom du titre de la pièce.
        play.name = remaining.substr(0, idx);

        // On retire le titre de la pièce et le "#" d'après.
        remaining = remaining.substr(idx + 1);

        // Le début de remaining correspond au type de pièce (ex: COMEDIE).
        idx = remaining.find("#");
        play.kind = remaining.substr(0, idx);
        remaining = remaining.substr(idx + 1);

        // Il y a le contenu de la pièce, jusqu'à "#FIN#"
        idx = remaining.find("#FIN#");
        play.content = remaining.substr(0, idx);
        remaining = remaining.substr(idx + 5);

        // Retour au début, où l'on cherche le prochain #DEBUT#
    }

    return plays;
}

int64_t exo5::CountLetters(String const& play)
{
    int64_t letterCount = 0;

    for (char c : play)
    {
        if (std::isalpha(c))
            letterCount++;
    }

    return letterCount;
}

exo5::CharCountList exo5::GetSortedLetterCount(String const& play)
{ 
    CharCountList res;

    for (char c : play)
    {
        if (std::isalpha(c))
        {
            char l = std::toupper(c);
            bool found = false;

            for (CharCount cc : res)
            {
                if (c == cc.first)
                {
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                res.push_back({l, 1});
            }
        }
    }
    
#if 1
    res.sort(_OrderCharCount);
#endif
    return res;
}

int64_t exo5::CountWords(String const& play)
{
    int64_t wordCount = 0;
    String remaining{play};
    size_t idx = 0;
    while (idx != String::npos)
    {
        idx = 0;

        // Find next word.
        idx = remaining.find(" ");
        remaining = remaining.substr(idx + 1);
        idx = 0;
        // Find word end.
        idx = remaining.find(" ");

        if (remaining.substr(0, idx).length() > 1)
            wordCount++;
    }
    return wordCount;
}

exo5::StringList exo5::FindWords(String const& play)
{
    StringList sl = StringList();
    String remaining{play};
    size_t idx = 0;
    while (idx != String::npos)
    {
        idx = 0;

        // Find next word.
        while (idx < remaining.length() - 1 &&
               (std::isalpha(remaining[idx]) || !std::isalpha(remaining[idx + 1])))
            idx++;

        remaining = remaining.substr(idx + 1);
        idx = 0;
        // Find word end.
        while (idx < remaining.length() - 1 &&
               (!std::isalpha(remaining[idx]) || std::isalpha(remaining[idx + 1])))
            idx++;
        idx++;
        
        sl.push_back(remaining.substr(0, idx));

        idx = remaining.find(" ");
    }
    return sl;
}

exo5::StringList exo5::FindUniqueWords(StringList const& words)
{
    StringList sl = StringList();
    for (String word : words)
    {
        bool contains = false;
        for (String uniqueWord : sl)
        {
            if (word == uniqueWord)
            {
                contains = true;
                break;
            }
        }
        if (!contains)
            sl.push_back(word);
    }
    return sl;
}

exo5::StringCountList exo5::GetSortedWordCount(StringList const& words)
{
    StringCountList res;

    for (String word : words)
    {
        bool found = false;
        
        for (StringCount& word2 : res)
        {
            if (word2.first == word)
            {
                word2.second++;
                found = true;
                break;
            }
        }

        if (!found)
        {
            res.push_back({word, 1});
        }
    }

#if 1
    res.sort(_OrderWordCount);
#endif
    return res;
}

bool exo5::_OrderCharCount(CharCount const& a, CharCount const& b)
{
    return a.second > b.second;
}

bool exo5::_OrderWordCount(StringCount const& a, StringCount const& b)
{
    return a.second > b.second;
}
