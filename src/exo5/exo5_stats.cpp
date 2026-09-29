#include <exo5/exo5_stats.h>

#include <algorithm>

std::vector<exo5::Play> exo5::FindAllPlays(String const& moliere)
{
    std::vector<exo5::Play> plays;

    String remaining{moliere};
    while (true)
    {
        // On enlève tout ce qui est avant "#DEBUT#" (inclus)
        size_t idx = remaining.find("#DEBUT#");
        if (idx == String::npos)
            break;
        exo5::Play& play = plays.emplace_back();

        // On cherche la position du premier caractère de la prochaine occurrence de "#DEBUT#"
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
    int64_t n = 0;
    for (char c : play)
    {
        if (std::isalpha(c))
        {
            n++;
        }
    }
    return n;
}

exo5::CharCountList exo5::GetSortedLetterCount(String const& play)
{ 
    CharCountList res;
    // TODO
    for (char c : play)
    {
        if (!std::isalpha(c))
            continue;

        c = tolower(c);

        bool exists = false;
        for (exo5::CharCount& p : res)
        {
            if (c == p.first)
            {
                exists = true;
                p.second++;
            }
        }
        if (!exists)
        {
            res.push_back(std::pair<char, int64_t>{c, 1});
        }
    }

#if 1
    std::sort(res.begin(), res.end(), _OrderCharCount);
#endif
    return res;
}

int64_t exo5::CountWords(String const& play)
{
    int n = 0;
    bool isInWord = false;
    for (char c : play)
    {
        if (std::isalpha(c))
        {
            isInWord = true;
        }
        else
        {
            if (isInWord)
            {
                isInWord = false;
                n++;
            }
        }
    }

    return n;
}

exo5::StringList exo5::FindWords(String const& play)
{
    exo5::StringList l;

    std::string s = "";
    String sView;
    bool isInWord = false;
    for (char c : play)
    {
        if (std::isalpha(c))
        {
            isInWord = true;
            s += std::tolower(c);
        }
        else
        {
            if (isInWord)
            {
                sView = {s};
                l.push_back(sView);
                s = "";
                isInWord = false;
            }
        }
    }

    return l;
}

exo5::StringList exo5::FindUniqueWords(StringList const& words)
{
    StringList l;
    bool isInList = false;
    for (String s : words)
    {
        for (String s2 : l)
        {
            if (s == s2)
            {
                isInList = true;
            }
        }
        if (!isInList)
        {
            l.push_back(s);
        }
        isInList = false;
    }

    return l;
}

exo5::StringCountList exo5::GetSortedWordCount(StringList const& words)
{
    return {};
}

bool exo5::_OrderCharCount(std::pair<char, uint64_t> firstElt, std::pair<char, uint64_t> secondElt)
{
    return (firstElt.second > secondElt.second);
}


