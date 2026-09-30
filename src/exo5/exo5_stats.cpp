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
    int64_t count = 0;
    for (char c : play){
        if (std::isalpha(c)){
            count += 1;
        }
    }
    return count;
}

bool _OrderCharCount(exo5::CharCount a, exo5::CharCount b) {
    return a.second > b.second;
};

exo5::CharCountList exo5::GetSortedLetterCount(String const& play)
{ 
    CharCountList res;

    for (char c : play){
        if (std::isalpha(c)){
            c = std::toupper(c);
            bool bFound = false;
            for (CharCount& cc : res){
                if (cc.first == c)
                {
                    cc.second+= 1;
                    bFound = true;
                }
            }
            if (!bFound){
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
        if (std::isalpha(c))
        {
            if (!bPrevLetter)
            {
                count += 1;
            }
        }
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
    for (size_t idx = 0; idx < play.size(); idx += 1){
        char c = play[idx];
        if (std:: isalpha(c)){
            if (!bPrevLetter)
            {
                idxWordBegin = idx;
            }
               
        }
        else
        {
            if (bPrevLetter)
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
    StringList strList;
    bool bWordExist;
    for (String str : words)
    {
        bWordExist = false;
        for (String str2 : strList)
        {
            if (str == str2)
            {
                bWordExist = true;
            }
        }
        if (!bWordExist)
        {
            strList.push_back(str);
        }
    }
    return strList;
}

bool _OrderLetterCount(exo5::StringCount a, exo5::StringCount b)
{
    return a.second > b.second;
};

exo5::StringCountList exo5::GetSortedWordCount(StringList const& words)
{
    StringCountList strList;

    for (String str : words)
    {
        bool bFound = false;
        for (StringCount& str2 : strList)
        {
            if (str2.first == str)
            {
                str2.second += 1;
                bFound = true;
            }
        }
        if (!bFound)
        {
            StringCount str2{str, 1};
            strList.push_back(str2);
        }
    }
#if 1
    std::sort(strList.begin(), strList.end(), _OrderLetterCount);
#endif
    return strList;
}