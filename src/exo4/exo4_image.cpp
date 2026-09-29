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
    uint32_t  
        width = *(uint32_t*)(pFile + 0x0012);
    uint32_t height = *(uint32_t*)(pFile + 0x0016);

    uint16_t bitsPerPixel = *(uint16_t*)(pFile + 0x001C);
    if (bitsPerPixel != 8 && bitsPerPixel != 24)
        return {};

    // Destination
    std::vector<jv::util::Color> pixels;
    pixels.resize(width * height);
    jv::util::Color* pPixels = pixels.data();

    // A ENLEVER
    /*float f = jv::util::RandomFloat(0, 6.28);
    pPixels[0].r = 128;
    pPixels[0].g = 127.5 + 127.5 * std::sin(f);
    pPixels[0].b = 127.5 + 127.5 * std::cos(f);
    pPixels[0].a = 255;*/

    if (bitsPerPixel == 24)
    {

        // 3 octets par pixel, BGR
        for (size_t i = 0; i <  width; i++)
        {
            for (size_t y = 0; y < height; y++)
            {
                int index = y * width + i;
                int indexY = ((height - y - 1) * width + i) * 3;

                pPixels[index].b = (pFile + dataOffset)[indexY];
                pPixels[index].g = (pFile + dataOffset)[indexY +1];
                pPixels[index ].r = (pFile + dataOffset)[indexY +2];
                pPixels[index ].a = 255;
              
            }
        }
    }
    else if (bitsPerPixel == 8)
    {
        uint16_t numColorUsed = *(uint16_t*)(pFile + 0x002E);

        for (size_t i = 0; i < width; i++)
        {
            for (size_t y = 0; y < height; y++)
            {
                int index = y * width + i;
                int indexY = ((height - y - 1) * width + i) * 1;

                int indexColor = (pFile + dataOffset)[indexY];

                pPixels[index].b = (pFile + 0x0036)[4*indexColor];
                pPixels[index].g = (pFile + 0x0036)[4*indexColor + 1];
                pPixels[index].r = (pFile + 0x0036)[4*indexColor  + 2];
                pPixels[index].a = 255;
            }
        }
    }

    jv::util::Vec2 pxSize = {width, height};
    jv::gpu::Texture* pTexture = jv::gpu::CreateTexture(imageName, pxSize, pixels);
    return {pTexture, pxSize};
}
