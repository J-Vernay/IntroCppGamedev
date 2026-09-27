#include <jv/jv.h>

#include <Windows.h>

std::vector<unsigned char> jv::util::LoadAsset(std::string_view name)
{
    std::string filepath;
    filepath += "assets/";
    filepath += name;

    HANDLE hFile;
    for (int i = 0; i < 5; ++i)
    {
        // Est-ce qu'il y a assets/NAME dans le dossier courant ?
        hFile = CreateFileA(filepath.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL, nullptr);
        if (hFile != INVALID_HANDLE_VALUE)
            break;
        // Réessayer depuis le dossier parent.
        filepath = "../" + filepath;
    }
    if (hFile == INVALID_HANDLE_VALUE)
        return {};
    DWORD size, sizeHigh;
    size = GetFileSize(hFile, &sizeHigh);
    if (sizeHigh > 0)
        return {}; // On supporte pas les fichiers de plus de 4GB.

    std::vector<unsigned char> file;
    file.resize(size);
    if (!ReadFile(hFile, file.data(), file.size(), nullptr, nullptr))
        file.clear(); // Erreur de lecture.
    CloseHandle(hFile);
    return file;
}