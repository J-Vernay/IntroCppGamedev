#include <exo4/exo4_image.h>
#include <cmath>

// Informations sur le format BMP:
// https://www.ece.ualberta.ca/~elliott/ee552/studentAppNotes/2003_w/misc/bmp_file_format/bmp_file_format.htm

exo4::Image exo4::LoadImageAsset(std::string_view imageName)
{
    std::vector<unsigned char> file = jv::util::LoadAsset(imageName);
    if (file.size() < 14)
        return {}; // Le fichier est trop petit, il ne peut pas contenir un BMP.
    if (file[0] != 'B' || file[1] != 'M')
        return {};

    unsigned char* pFile = file.data();
    uint32_t fileSize = *(uint32_t*)(pFile + 0x0002);
    if (fileSize > file.size())
        return {}; // Fichier trop petit
    uint32_t dataOffset = *(uint32_t*)(pFile + 0x000A);

    // Interprétation des données
    uint32_t v1 = *(uint32_t*)(pFile + 0x0012);
    uint32_t v2 = *(uint32_t*)(pFile + 0x0016);

    uint16_t v3 = *(uint16_t*)(pFile + 0x001C);
    if (v3 != 8 && v3!= 24)
        return {};

    // Destination
    std::vector<jv::util::Color> pixels;
    pixels.resize(1);
    jv::util::Color* pPixels = pixels.data();

    // A ENLEVER
    float f = jv::util::RandomFloat(0, 6.28);
    pPixels[0].r = 128;
    pPixels[0].g = 127.5 + 127.5 * std::sin(f);
    pPixels[0].b = 127.5 + 127.5 * std::cos(f);
    pPixels[0].a = 255;

    if (v3 == 24)
    {
        // 3 octets par pixel, BGR
    }
    else if (v3 == 8)
    {
        // 1 octet par pixel, index vers la "colortable"
    }

    jv::util::Vec2 pxSize = {1, 1};
    jv::gpu::Texture* pTexture = jv::gpu::CreateTexture(imageName, pxSize, pixels);
    return {pTexture, pxSize};
}
