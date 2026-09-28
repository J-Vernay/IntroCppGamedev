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
    uint32_t width = *(uint32_t*)(pFile + 0x0012);
    uint32_t height = *(uint32_t*)(pFile + 0x0016);

    uint16_t bitsPerPixel = *(uint16_t*)(pFile + 0x001C);
    if (bitsPerPixel != 8 && bitsPerPixel!= 24)
        return {};

    // Destination
    std::vector<jv::util::Color> pixels;
    pixels.resize(width * height);
    jv::util::Color* pPixels = pixels.data();

    if (bitsPerPixel == 24)
    {
        // 3 octets par pixel, BGR
        
        for (uint32_t i = 0; i < width * height; i++)
        {
            uint32_t w = i % width;
            uint32_t h = i / width;
            uint32_t p = (height - h - 1) * width + w;
            pPixels[p].b = *(uint32_t*)(pFile + 0x0036 + 3 * i);
            pPixels[p].g = *(uint32_t*)(pFile + 0x0036 + 3 * i + 1);
            pPixels[p].r = *(uint32_t*)(pFile + 0x0036 + 3 * i + 2);
            pPixels[p].a = 255;
        }
    }
    else if (bitsPerPixel == 8)
    {
        // 1 octet par pixel, index vers la "colortable"


    }

    jv::util::Vec2 pxSize = {width, height};
    jv::gpu::Texture* pTexture = jv::gpu::CreateTexture(imageName, pxSize, pixels);
    return {pTexture, pxSize};
}
