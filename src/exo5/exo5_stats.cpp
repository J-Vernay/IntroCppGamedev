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
    int numChar = 0;
    for (char c : play)
    {
        if (std::isalpha(c) != 0)
        {
            numChar++;
        }
    }
    return numChar;
}

exo5::CharCountList exo5::GetSortedLetterCount(String const& play)
{
    bool found;
    CharCountList res;
    for (char c : play)
    {
        if (std::isalpha(c) != 0 && std::toupper(c))
        {
            found = false;
            for (CharCount& sc : res)
            {
                if (sc.first == std::toupper(c))
                {
                    found = true;
                    sc.second++;
                }
            }
            if (!found)
                res.push_back(CharCount(std::toupper(c), 1));
        }
    }

#if 1

    res.sort(_OrderCharCount);
    //std::sort(res.begin(), res.end(), _OrderCharCount);
#endif
    return res;
}

bool exo5::_OrderCharCount(exo5::CharCount element1, exo5::CharCount element2)
{
    if (element1.second > element2.second)
        return true;
    return false;
}

int64_t exo5::CountWords(String const& play)
{
    int numWords = 0;
    char lastLetter = 'a';
    for (char c : play)
    {
        if (!std::isalpha(c) && std::isalpha(lastLetter))
        {
            numWords++;
        }
        lastLetter = c;
    }

    return numWords;
}

exo5::StringList exo5::FindWords(String const& play)
{
    exo5::StringList listString;
    int lastStart;
    int index= -1;
    char lastLetter = '2';

    for (char c : play)
    {
        index++;
        if (!std::isalpha(c) && std::isalpha(lastLetter))
        {
            listString.push_back(play.substr(lastStart, index-lastStart));
        }
        else if (std::isalpha(c) && !std::isalpha(lastLetter))
        {
            lastStart = index;
        }
        lastLetter = c;
    }
        return listString;
}

exo5::StringList exo5::FindUniqueWords(StringList const& words)
{
    StringList uniqueWords;
    for (String currentWord : words)
    {
        int cnt = std::count(uniqueWords.begin(), uniqueWords.end(), currentWord);

        if (cnt >= 1)
            continue;
        else
            uniqueWords.push_back(currentWord);
    }
    return uniqueWords;
}

exo5::StringCountList exo5::GetSortedWordCount(StringList const& words)
{
    StringCountList wordsSorted;
    bool find = false;
    //for (size_t i = 0; i < words.size(); i++)
    for (String currentWord : words)
    {
        //exo5::String currentWord = words[i];
        find = false;
        //for (size_t i = 0; i < wordsSorted.size(); i++)
        for (StringCount& sc : wordsSorted)
        {
            if (sc.first == currentWord)
            {
                sc.second++;
                find = true;
            }
        }
        if (!find)
        {
            wordsSorted.push_back(StringCount(currentWord, 1));
        }
    }

    wordsSorted.sort(_OrderWordCount);
    //std::sort(wordsSorted.begin(), wordsSorted.end(), _OrderWordCount);
    return wordsSorted;
}

bool exo5::_OrderWordCount(exo5::StringCount element1, exo5::StringCount element2)
{
    if (element1.second > element2.second)
        return true;
    return false;
}