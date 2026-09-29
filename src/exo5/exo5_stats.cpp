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

        // On récupère le nom du titre de la pièce
        play.name = remaining.substr(0, idx);

        // On retire le titre de la pièce et le "#" d'après
        remaining = remaining.substr(idx + 1);

        // Le début de remaining correspond au type de pièce (ex: COMEDIE)
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
    for (unsigned char c : play)
    {
        if (std::isalpha(c))
            count++;
    }
    return count;
}

bool _OrderCharCount(const exo5::CharCount& a, const exo5::CharCount& b)
{
    if (a.second != b.second)
    {
        return a.second > b.second;
    }

    return a.first < b.first;
}

exo5::CharCountList exo5::GetSortedLetterCount(String const& play)
{ 
    CharCountList res;
    int64_t counts[26] = {0};

    for (unsigned char c : play)
    {
        if (std::isalpha(c))
        {
            if (res.)
        }
    }

    
#if 0
    std::sort(res.begin(), res.end(), _OrderCharCount);
#endif
    return res;
}

int64_t exo5::CountWords(String const& play)
{
    int64_t count = 0;
    bool isInWord = false;

    for (unsigned char c : play)
    {
        if (std::isalpha(c))
        {
            if (!isInWord)
            {
                count++;
                isInWord = true;
            }
        }
        else
        {
            isInWord = false;
        }
    }

    return count;
}

exo5::StringList exo5::FindWords(String const& play)
{
    StringList strings;

    bool isInWord = false;
    size_t wordStart;

    for (size_t i = 0; i < play.size(); i++)
    {
        if (std::isalpha((unsigned char)play[i]))
        {
            if (!isInWord)
            {
                wordStart = i;
                isInWord = true;
            }
        }
        else
        {
            if (isInWord)
            {
                strings.push_back(play.substr(wordStart, i - wordStart));
            }
            isInWord = false;
        }
    }
    return strings;
}

exo5::StringList exo5::FindUniqueWords(StringList const& words)
{
    return {};
}

exo5::StringCountList exo5::GetSortedWordCount(StringList const& words)
{
    return {};
}