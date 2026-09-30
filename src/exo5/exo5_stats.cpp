#include <exo5/exo5_stats.h>

#include <algorithm>

#include <iostream>

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
    int64_t count = 0;
    for (char c : play)
        if (std::isalpha(c))
            count++;
    return count;
}

bool _OrderCharCount(exo5::CharCount a, exo5::CharCount b)
{
    return a.second > b.second;
}

exo5::CharCountList exo5::GetSortedLetterCount(String const& play)
{ 
    CharCountList res;

    for (char c : play)
        if (std::isalpha(c))
        {
            char letter = std::tolower(c);
            bool found = false;
            for (char index = 0; index < res.size(); index++)
            {
                if (res[index].first == letter)
                {
                    found = true;
                    res[index].second++;
                    break;
                }
            }
            if (!found)
                res.push_back({letter, 1});
        }
    
#if 1
    std::sort(res.begin(), res.end(), _OrderCharCount);
#endif
    return res;
}

int64_t exo5::CountWords(String const& play)
{
    int count = 0;
    for (int i = 0; i < play.size(); i++)
    {
        if (std::isalpha(play[i]))
        {
            count++;

            while (i < play.size() && std::isalpha(play[i]))
                i++;

        }
    }
    return count;
}

exo5::StringList exo5::FindWords(String const& play)
{
    StringList res;
    
    for (int i = 0; i < play.size(); i++)
    {
        int indexStart = i;
        // get every letter in the word
        while (i < play.size() && std::isalpha(play[i]))
        {
            i++;
        }
        // push the word to the word list
        if (i != indexStart)
        {
            res.push_back(play.substr(indexStart, i - indexStart));
        }
    }
    return res;
}

exo5::StringList exo5::FindUniqueWords(StringList const& words)
{
    StringList res;
    
    for (String word : words)
    {
        bool found = false;
        for (int i = 0; i < res.size(); i++)
        {
            if (res[i] == word)
            {
                found = true;
                break;
            }
        }
        if (!found)
            res.push_back(word);
    }

    return res;
}
bool _OrderWordCount(exo5::StringCount a, exo5::StringCount b)
{
    return a.second > b.second;
}

exo5::StringCountList exo5::GetSortedWordCount(StringList const& words)
{
    StringCountList res;

    for (String word : words)
    {
        bool found = false;
        for (int i = 0; i < res.size(); i++)
        {
            if (res[i].first == word)
            {
                found = true;
                res[i].second++;
                break;
            }
        }
        if (!found)
            res.push_back({word, 1});
    }

    std::sort(res.begin(), res.end(), _OrderWordCount);

    return res;
}