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

    uint16_t v3 = *(uint16_t*)(pFile + 0x001C);
    if (v3 != 8 && v3!= 24)
        return {};


    // Destination
    std::vector<jv::util::Color> pixels;
    pixels.resize(width * height);
    jv::util::Color* pPixels = pixels.data();

    if (v3 == 24)
    {
        for (uint32_t row = 0; row < height; row++)
        {
            uint32_t invrow = height - 1 - row;

            for (uint32_t col = 0; col < width; col++)
            {
                uint32_t src_i = invrow * width + col;
                pPixels->b = *(pFile + dataOffset + src_i * 3);
                pPixels->g = *(pFile + dataOffset + src_i * 3 + 1);
                pPixels->r = *(pFile + dataOffset + src_i * 3 + 2);
                pPixels->a = 255;
                pPixels += 1;
            }
        }

    }
    else if (v3 == 8)
    {
        for (uint32_t row = 0; row < height; row++)
        {
            uint32_t invrow = height - 1 - row;

            for (uint32_t col = 0; col < width; col++)
            {

                uint32_t src_i = invrow * width + col;
                uint8_t idxColor = *(pFile + dataOffset + src_i);

                pPixels->b = *(pFile + 0x0036 + idxColor * 4);
                pPixels->g = *(pFile + 0x0036 + idxColor * 4 + 1);
                pPixels->r = *(pFile + 0x0036 + idxColor * 4 + 2);
                pPixels->a = 255;
                pPixels += 1;
            }
        }
    }

    /*
    for (jv::util::Color& c : pixels)
    {
        if (c.r == 255 && c.g == 0 && c.b = 255 && c.a == 255)
        {
            c.r = 0;
            c.a = 0;
            c.b = 0;
            c.g = 0;
        }
    }
    */
    
    jv::util::Vec2 pxSize = {1, 1};
    jv::gpu::Texture* pTexture = jv::gpu::CreateTexture(imageName, pxSize, pixels);
    return {pTexture, pxSize};
}