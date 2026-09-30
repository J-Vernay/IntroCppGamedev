#include <exo5/exo5_stats.h>

#include <algorithm>

#include <unordered_set>

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
    {
        if(isalpha(c)) count++;
    }

    return count;
}

exo5::CharCountList exo5::GetSortedLetterCount(String const& play)
{ 
    CharCountList res;
    
    for (char c : play)
    {
        if (!isalpha(c))
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
            res.push_back(std::pair<char, int64_t>(c, 1));
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

    bool inWord = false;

    for (char c : play)
    {
        if (isalpha(c))
        {
            inWord = true;
        }
        else
        {
            if (inWord)
            {
                count++;
                inWord = false;
            }
        }
    }

    return count;
}

exo5::StringList exo5::FindWords(String const& play)
{
    StringList list;

    bool inWord = true;

    int indexDebut = 0;
    int indexFin = 0;

    for (int i = 0; i < play.size(); i++)
    {
        if (isalpha(play[i]))
        {
            if (inWord)
                continue;

            inWord = true;
            indexDebut = i;
        }
        else
        {
            if (inWord)
            {
                indexFin = i;
                list.push_back(play.substr(indexDebut, indexFin - indexDebut));
                inWord = false;
            }
        }
    }

    return list;
}

exo5::StringList exo5::FindUniqueWords(StringList const& words)
{
    StringList list;

   std ::unordered_set<String> hashsetString;

    for (String word : words)
    {
        hashsetString.insert(word);
    }

    for (String word : hashsetString)
    {
        list.push_back(word);
    }

    return list;
}

exo5::StringCountList exo5::GetSortedWordCount(StringList const& words)
{
    StringCountList list;

    StringCount currentStringCount;

    std::unordered_map<String,int64_t> dicoStringCount;

    for (String word : words)
    {
        dicoStringCount[word] += 1;
    }

    for (const std::pair<exo5::String, int64_t>& word : dicoStringCount)
    {
        list.push_back(word);
    }

    std::sort(list.begin(), list.end(), [](StringCount first, StringCount second) {
        return first.second > second.second;
    });

    return list;
}

bool exo5::_OrderCharCount(
    const std::pair<char, int64_t> first, const std::pair<char, int64_t> second)
{
    return first.second > second.second;
}
