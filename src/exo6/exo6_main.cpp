
#include <algorithm>
#include <chrono>
#include <deque>
#include <format>
#include <fstream>
#include <iostream>
#include <list>
#include <string_view>
#include <thread>
#include <vector>

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

using Clock = std::chrono::steady_clock;
using Millisec = std::chrono::duration<double, std::milli>;

std::ofstream g_outStat;
int64_t g_dummy;

template <typename TContainer> void TestContainer(std::string_view testKind)
{
    std::cout << "\n\n==================== " << testKind << " ====================\n\n";
    TContainer vs;
    for (size_t size : {1, 2, 4})
    {
        vs.resize(size);
        std::cout << std::format("----- resize({}) -----\n", size);
        size_t i = 0;
        for (auto& v : vs)
        {
            std::cout << std::format("&vs[{: 4}] = {} ( dec: {} )\n", i, (void*)&v, (intptr_t)&v);
            i += 1;
        }
    }
    for (size_t size : {100, 1000, 10000})
    {
        vs.resize(size);
        std::cout << std::format("----- resize({}) -----\n", size);
        std::cout << std::format(
            "&vs[{:4}] = {} ( dec: {} )\n", 0, (void*)&vs.front(), (intptr_t)&vs.front());
        std::cout << std::format(
            "&vs[{:4}] = {} ( dec: {} )\n", size - 1, (void*)&vs.back(), (intptr_t)&vs.back());
        std::cout << std::format("diff = {} (dec)\n", (intptr_t)&vs.back() - (intptr_t)&vs.front());
    }
    vs.clear();
    for (size_t size = 10000; size <= 100000; size += 10000)
    {
        vs.resize(size);

        intptr_t accum = 0;
        Millisec ms[3];
        for (int i = 0; i < 3; ++i)
        {
            Clock::time_point tic = Clock::now();
            for (auto& v : vs)
                accum += *(unsigned char*)&v;
            Clock::time_point tac = Clock::now();
            ms[i] = tac - tic;

            std::this_thread::sleep_for(Millisec(1));
        }
        Millisec msMin = std::ranges::min(ms);

        size_t elemSize = sizeof(typename TContainer::value_type);
        g_outStat << testKind << "-" << size << "," << testKind.substr(0, testKind.find('-')) << ","
                  << testKind << "," << elemSize << "," << size << "," << elemSize * size << ","
                  << msMin.count() << "\n";

        g_dummy = accum;
    }
}

template <int N> struct Struct
{
    char data[N];
};

int main()
{
    std::setlocale(LC_ALL, ".UTF8");

    std::string outStatPath = GetUserDocumentsFolder() + "/exo6_timing_" + GetTimeStamp() + ".csv";
    g_outStat.open(outStatPath);
    g_outStat << "TestName,Container,TestKind,ElemSize,Count,DataSize,IterTimeMs\n";

    TestContainer<std::vector<char>>("vector-char");
    TestContainer<std::vector<Struct<256>>>("vector-Struct256");
    TestContainer<std::vector<Struct<1024>>>("vector-Struct1024");
    TestContainer<std::vector<Struct<4096>>>("vector-Struct4096");

    TestContainer<std::deque<char>>("deque-char");
    TestContainer<std::deque<Struct<256>>>("deque-Struct256");
    TestContainer<std::deque<Struct<1024>>>("deque-Struct1024");
    TestContainer<std::deque<Struct<4096>>>("deque-Struct4096");

    TestContainer<std::string>("string");

    TestContainer<std::list<char>>("list-char");
    TestContainer<std::list<Struct<256>>>("list-Struct256");
    TestContainer<std::list<Struct<1024>>>("list-Struct1024");
    TestContainer<std::list<Struct<4096>>>("list-Struct4096");

    g_outStat.close();

    std::cout << "\nDonnées de timing écrites dans le fichier:\n";
    std::cout << outStatPath << "\n\n";

    return EXIT_SUCCESS;
}