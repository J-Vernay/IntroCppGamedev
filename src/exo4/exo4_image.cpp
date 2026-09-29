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
    uint32_t L = *(uint32_t*)(pFile + 0x0012);
    uint32_t H = *(uint32_t*)(pFile + 0x0016);

    uint16_t v3 = *(uint16_t*)(pFile + 0x001C);
    if (v3 != 8 && v3!= 24)
        return {};

    // Destination
    std::vector<jv::util::Color> pixels;
    pixels.resize(L*H);
    jv::util::Color* pPixels = pixels.data();

    if (v3 == 24)
    {

        for (uint32_t y = 0; y < H; y++)
        {
            for (uint32_t x = 0; x < L; x++)
            {
                uint32_t index = (y * L + x) * 3;
                pPixels[((H - y - 1) * L) + x].b = *(pFile + dataOffset + index);
                pPixels[((H - y - 1) * L) + x].g = *(pFile + dataOffset + index + 1);
                pPixels[((H - y - 1) * L) + x].r = *(pFile + dataOffset + index + 2);
                pPixels[((H - y - 1) * L) + x].a = 255;
            }
        }
    }
    else if (v3 == 8)
    {

        unsigned char* ColorTable = (pFile + 0x0036);

        for (int i = 0; i < 256; i++)
        {
            unsigned char* index = (ColorTable + i * 4);
            if (*index == 255 && *(index + 1) == 0 && *(index + 2) == 255 && *(index + 3) == 255)
            {
                *(index) = 0;
                *(index + 2) = 0;
            }
        }

        for (uint32_t y = 0; y < H; y++)
        {
            for (uint32_t x = 0; x < L; x++)
            {
                uint32_t index = (y * L + x);
                uint32_t colorIndex = *(pFile + dataOffset + index);
                unsigned char* finalIndex = (ColorTable + colorIndex * 4);
                if (*finalIndex == 255 && *(finalIndex + 1) == 0 && *(finalIndex + 2) == 255 &&
                    *(finalIndex + 3) == 255)
                {
                    pPixels[((H - y - 1) * L) + x].b = 0;
                    pPixels[((H - y - 1) * L) + x].g = 0;
                    pPixels[((H - y - 1) * L) + x].r = 0;
                    pPixels[((H - y - 1) * L) + x].a = 255;
                }
                else
                {
                    pPixels[((H - y - 1) * L) + x].b = *finalIndex;
                    pPixels[((H - y - 1) * L) + x].g = *(finalIndex + 1);
                    pPixels[((H - y - 1) * L) + x].r = *(finalIndex + 2);
                    pPixels[((H - y - 1) * L) + x].a = 255;
                }
            }
        }
    }

    jv::util::Vec2 pxSize = {L, H};
    jv::gpu::Texture* pTexture = jv::gpu::CreateTexture(imageName, pxSize, pixels);
    return {pTexture, pxSize};
}
