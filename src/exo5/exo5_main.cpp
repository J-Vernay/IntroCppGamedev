#include <chrono>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <ratio>

#include <exo5/exo5_stats.h>

std::string LoadAssetText(std::string_view name)
{
    std::string path{"assets/"};
    path.append(name);
    std::FILE* f = nullptr;
    for (int i = 0; i < 5; ++i)
    {
        f = std::fopen(path.c_str(), "r");
        if (f)
            break;
        path = "../" + path;
    }
    if (!f)
        return {};

    std::string content;
    std::string buffer;
    buffer.resize(4096);
    while (!std::feof(f))
    {
        int nread = std::fread(buffer.data(), 1, buffer.size(), f);
        content.append(buffer.data(), nread);
    }
    return content;
}

std::string GetUserDocumentsFolder()
{
#if _WIN64
    std::string path = getenv("USERPROFILE");
    path.append("/Documents");
#else
    std::string path = getenv("XDG_DOCUMENTS_DIR");
#endif

    for (char& c : path)
        if (c == '\\')
            c = '/';
    return path;
}

std::string GetTimeStamp()
{
    std::time_t time = std::time(nullptr);
    std::tm* tm = std::localtime(&time);
    if (!tm)
        return "";
    std::string buf;
    buf.resize(256);
    int size = std::strftime(buf.data(), buf.size(), "%Y-%m-%d-%H-%M-%S", tm);
    buf.resize(size);
    return buf;
}

using namespace exo5;

using Clock = std::chrono::steady_clock;
using TimeMs = std::chrono::duration<double, std::milli>;

struct PlayStats
{
    String name;
    String kind;
    int64_t charCount;
    int64_t letterCount;
    CharCountList letterCounts;
    int64_t wordCount;
    StringList words;
    StringList uniqueWords;
    StringCountList wordCounts;

    TimeMs msCountLetters;
    TimeMs msGetSortedLetterCount;
    TimeMs msCountWords;
    TimeMs msFindWords;
    TimeMs msFindUniqueWords;
    TimeMs msGetSortedWordCount;
};

int main()
{
    std::setlocale(LC_ALL, ".UTF8");

    std::string moliere = LoadAssetText("moliere_integrale.txt");
    if (moliere.empty())
        return EXIT_FAILURE;

    std::cout << "Longueur de l'intégrale de Molière : " << moliere.size() << "\n";
    std::cout << "==================== DEBUT EXTRAIT ====================\n";
    std::cout << moliere.substr(0, 300) << "\n";
    std::cout << "==================== FIN EXTRAIT ====================\n";

#if 1

    std::vector<Play> plays = FindAllPlays(moliere);

    std::vector<PlayStats> stats;

    PlayStats moyenne{"!!! MOYENNE !!!", "MOYENNE"};

    for (Play const& play : plays)
    {
        std::cout << "Traitement de " << play.name << "...\n";

        Clock::time_point tic;

        PlayStats& stat = stats.emplace_back();
        stat.name = play.name;
        stat.kind = play.kind;
        stat.charCount = play.content.size();

        tic = Clock::now();
        stat.letterCount = CountLetters(play.content);
        stat.msCountLetters = Clock::now() - tic;

        tic = Clock::now();
        stat.letterCounts = GetSortedLetterCount(play.content);
        stat.msGetSortedLetterCount = Clock::now() - tic;

        tic = Clock::now();
        stat.wordCount = CountWords(play.content);
        stat.msCountWords = Clock::now() - tic;

        tic = Clock::now();
        stat.words = FindWords(play.content);
        stat.msFindWords = Clock::now() - tic;

        tic = Clock::now();
        stat.uniqueWords = FindUniqueWords(stat.words);
        stat.msFindUniqueWords = Clock::now() - tic;

        tic = Clock::now();
        stat.wordCounts = GetSortedWordCount(stat.words);
        stat.msGetSortedWordCount = Clock::now() - tic;

        moyenne.charCount += stat.charCount;
        moyenne.letterCount += stat.letterCount;
        moyenne.wordCount += stat.wordCount;
        moyenne.msCountLetters += stat.msCountLetters;
        moyenne.msGetSortedLetterCount += stat.msGetSortedLetterCount;
        moyenne.msCountWords += stat.msCountWords;
        moyenne.msFindWords += stat.msFindWords;
        moyenne.msFindUniqueWords += stat.msFindUniqueWords;
        moyenne.msGetSortedWordCount += stat.msGetSortedWordCount;
    }

    moyenne.charCount /= plays.size();
    moyenne.letterCount /= plays.size();
    moyenne.wordCount /= plays.size();
    moyenne.msCountLetters /= plays.size();
    moyenne.msGetSortedLetterCount /= plays.size();
    moyenne.msCountWords /= plays.size();
    moyenne.msFindWords /= plays.size();
    moyenne.msFindUniqueWords /= plays.size();
    moyenne.msGetSortedWordCount /= plays.size();

    stats.push_back(moyenne);

    for (PlayStats const& stat : stats)
    {
        std::cout << "\n========== " << stat.name << " ==========\n";
        std::cout << "Type            : " << stat.kind << "\n";
        std::cout << "Nb caractères   : " << stat.charCount << "\n";
        std::cout << "Nb lettres      : " << stat.letterCount << "\n";
        std::cout << "Nb mots         : " << stat.wordCount << "\n";
        std::cout << "Nb mots uniques : " << stat.uniqueWords.size() << "\n";

        if (!stat.letterCounts.empty())
        {
            std::cout << "Top 10 lettres :\n";
            int i = 1;
            for (auto [str, count] : stat.letterCounts)
            {
                std::cout << i << ". " << str << " (" << count << ")\n";
                i += 1;
                if (i > 10)
                    break;
            }
        }

        if (!stat.wordCounts.empty())
        {
            std::cout << "Top 10 mots :\n";
            int i = 1;
            for (auto [str, count] : stat.wordCounts)
            {
                std::cout << i << ". " << str << " (" << count << ")\n";
                i += 1;
                if (i > 10)
                    break;
            }
        }
    }

    std::cout << "Moyenne msCountLetters:         " << moyenne.msCountLetters << "\n";
    std::cout << "Moyenne msGetSortedLetterCount: " << moyenne.msGetSortedLetterCount << "\n";
    std::cout << "Moyenne msCountWords:           " << moyenne.msCountWords << "\n";
    std::cout << "Moyenne msFindWords:            " << moyenne.msFindWords << "\n";
    std::cout << "Moyenne msFindUniqueWords:      " << moyenne.msFindUniqueWords << "\n";
    std::cout << "Moyenne msGetSortedWordCount:   " << moyenne.msGetSortedWordCount << "\n";

    std::cout << "\n========================================\n";

    std::string outStatPath = GetUserDocumentsFolder() + "/exo5_timing_" + GetTimeStamp() + ".csv";
    std::ofstream outStat{outStatPath};
    outStat << "Nom,NbCaracteres,NbLettres,NbMots,NbMotsUniques,msCountLetters,";
    outStat << "msGetSortedLetterCount,msCountWords,msFindWords,";
    outStat << "msFindUniqueWords,msGetSortedWordCount\n";
    for (PlayStats const& stat : stats)
    {
        outStat << stat.name << "," << stat.charCount << ",";
        outStat << stat.letterCount << "," << stat.wordCount << ",";
        outStat << stat.uniqueWords.size() << ",";
        outStat << stat.msCountLetters.count() << ",";
        outStat << stat.msGetSortedLetterCount.count() << ",";
        outStat << stat.msCountWords.count() << ",";
        outStat << stat.msFindWords.count() << ",";
        outStat << stat.msFindUniqueWords.count() << ",";
        outStat << stat.msGetSortedWordCount.count() << "\n";
    }

    std::cout << "\nDonnées de timing écrites dans le fichier:\n";
    std::cout << outStatPath << "\n\n";
#endif

    return EXIT_SUCCESS;
}