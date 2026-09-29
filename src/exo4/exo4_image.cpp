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

    // Bit per pixel
    uint16_t bpp = *(uint16_t*)(pFile + 0x001C);
    if (bpp != 8 && bpp != 24)
        return {};

    // Destination
    std::vector<jv::util::Color> pixels;
    pixels.resize(width * height);
    jv::util::Color* pPixels = pixels.data();

    // Pointeur vers les données de pixels
    unsigned char* pData = pFile + dataOffset;

    // Taille de chaque scanline dans les données (padding to 4 bytes, 21 bytes width => 24 bytes)
    uint32_t scanLineWidth;
    if (width % 4 == 0)
        scanLineWidth = width;
    else
        scanLineWidth = width + (4 - (width % 4));

    if (bpp == 24)
    {
        // 3 octets par pixel, BGR
        for (uint32_t y = 0; y < height; y++)
        {
            for (uint32_t x = 0; x < width; x++)
            {
                jv::util::Color color = {
                    *(unsigned char*)(pData + (y * width + x) * (bpp / 8) + 2),
                    *(unsigned char*)(pData + (y * width + x) * (bpp / 8) + 1),
                    *(unsigned char*)(pData + (y * width + x) * (bpp / 8)),
                    255
                };
                if (color.rgba == 4294902015 /* 255 0 255 255*/)
                    color = {0, 0, 0, 255};
                pixels[(height - 1 - y) * width + x] = color;
            }
        }
    }
    else if (bpp == 8)
    {
        jv::util::Color ColorTable[256];

        for (uint32_t i = 0; i < 256; i++)
            ColorTable[i] = *(jv::util::Color*)(pFile + 0x0036 + i * 4);

        // 1 octet par pixel, index vers la "colortable"
        for (uint32_t y = 0; y < height; y++)
        {
            for (uint32_t x = 0; x < width; x++)
            {
                jv::util::Color color = ColorTable[*(unsigned char*)(pData + (y * width + x) * (bpp / 8))];
                color.a = 255;

                if (color.rgba == 4294902015 /* 255 0 255 255*/)
                    color = {0, 0, 0, 255};

                pixels[(height - 1 - y) * width + x] = color;
            }
        }
    }

    jv::util::Vec2 pxSize = {width, height};
    jv::gpu::Texture* pTexture = jv::gpu::CreateTexture(imageName, pxSize, pixels);
    return {pTexture, pxSize};
}
