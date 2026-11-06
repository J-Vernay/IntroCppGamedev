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

    uint16_t bitByPixel = *(uint16_t*)(pFile + 0x001C);

    if (bitByPixel != 8 && bitByPixel!= 24)
        return {};

    // Destination
    std::vector<jv::util::Color> pixels;
    pixels.resize(width * height);
    jv::util::Color* pPixels = pixels.data();


    if (bitByPixel == 24)
    {
        // 3 octets par pixel, BGR
        for (uint32_t y = 0; y < height; y++)
        {
            for (uint32_t x = 0; x < width; x++)
            {
                uint32_t index = y * width + x ;
                uint32_t indexInverse = (height - y - 1) * width + x ;

                uint8_t blueColor = *(pFile + dataOffset + index * 0x0003);
                uint8_t greenColor = *(pFile + dataOffset + index * 0x0003 + 0x0001);
                uint8_t redColor = *(pFile + dataOffset + index * 0x0003 + 0x0002);
                uint8_t alpha = 255;

                bool isPureMagenta = redColor == 255 && greenColor == 0 && blueColor == 255;

                if (isPureMagenta)
                {
                    blueColor = redColor = greenColor = 0;
                    alpha = 255;
                }

                pPixels[indexInverse].b = blueColor;
                pPixels[indexInverse].g = greenColor;
                pPixels[indexInverse].r = redColor;
                pPixels[indexInverse].a = alpha;
            }
        }
    }
    else if (bitByPixel == 8)
    {
        // 1 octet par pixel, index vers la "colortable"

        unsigned char* colorTablePtr = (pFile + 0x0036);

        for (uint32_t y = 0; y < height; y++)
        {
            for (uint32_t x = 0; x < width; x++)
            {
                uint32_t index = y * width + x;

                uint32_t indexInverse = (height - y - 1) * width + x;

                uint32_t indexAddressColorTable = *(pFile + dataOffset + index);
                
                unsigned char* finalIndex = (colorTablePtr + indexAddressColorTable * 4);

                uint8_t redColor = *(finalIndex);
                uint8_t greenColor = *(finalIndex + 0x0001);
                uint8_t blueColor = *(finalIndex + 0x0002);
                uint8_t alpha = 255;

                bool isPureMagenta = redColor == 255 && greenColor == 0 && blueColor == 255;

                if (isPureMagenta)
                {
                    blueColor = redColor = greenColor = 0;
                    alpha = 255;
                }

                pPixels[indexInverse].r = redColor;
                pPixels[indexInverse].g = greenColor;
                pPixels[indexInverse].b = blueColor;
                pPixels[indexInverse].a = 255;
            }
        }
    }

    jv::util::Vec2 pxSize = {width, height};
    jv::gpu::Texture* pTexture = jv::gpu::CreateTexture(imageName, pxSize, pixels);
    return {pTexture, pxSize};
}
