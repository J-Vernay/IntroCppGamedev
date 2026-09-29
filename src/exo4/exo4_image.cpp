#include <exo4/exo4_image.h>
#include <cmath>
#include <ios>

// Informations sur le format BMP:
// https://www.ece.ualberta.ca/~elliott/ee552/studentAppNotes/2003_w/misc/bmp_file_format/bmp_file_format.htm

exo4::Image exo4::LoadImageAsset(std::string_view imageName)
{
    std::vector<unsigned char> file = jv::util::LoadAsset(imageName);
    if (file.size() < 60)
        return {};
    if (file[0] != 'B' || file[1] != 'M')
        return {};

    unsigned char* pFile = file.data();
    uint32_t fileSize = *(uint32_t*)(pFile + 0x0002);
    if (fileSize > file.size())
        return {}; 
    uint32_t dataOffset = *(uint32_t*)(pFile + 0x000A);

    uint32_t width = *(uint32_t*)(pFile + 0x0012);
    uint32_t height = *(uint32_t*)(pFile + 0x0016);

    uint16_t bpp = *(uint16_t*)(pFile + 0x001C);
    if (bpp != 8 && bpp!= 24)
        return {};

    std::vector<jv::util::Color> pixels;

    pixels.resize(width * height);

    jv::util::Color* pPixels = pixels.data();


    if (bpp == 24)
    {

        const uint32_t octetPixel = width * 3;

        const uint32_t decalageQuatre = (4 - octetPixel % 4) % 4;

        const uint32_t finalRawSize = octetPixel + decalageQuatre;

        const uint8_t* pSourcePixels = pFile + dataOffset;

        for (uint32_t y = 0; y < height; ++y)
        {
            const uint32_t sourceY = height - 1 - y;

            for (uint32_t x = 0; x < width; ++x)
            {
                const uint32_t sourceIndex = sourceY * finalRawSize + x * 3;

               
                const uint32_t destinationIndex = y * width + x;

                pPixels[destinationIndex].b = pSourcePixels[sourceIndex];
                pPixels[destinationIndex].g = pSourcePixels[sourceIndex + 1];
                pPixels[destinationIndex].r = pSourcePixels[sourceIndex + 2];
                pPixels[destinationIndex].a = 255;
            }
        }
    }

    else if (bpp == 8)
    {


        uint32_t fileSizeEight = *(uint32_t*)(pFile + 0x000E);
        
        const  uint8_t* palette = pFile + 14 + fileSizeEight;

        const uint32_t decalage = (width + 3) / 4 * 4;

        const uint8_t* pSourcePixels = pFile + dataOffset;

        for (uint32_t y = 0; y < height; ++y)
        {
            const uint32_t sourceY = height - y;

            for (uint32_t x = 0; x < width; ++x)
            {
                const uint32_t sourceIndex = sourceY * decalage + x;

                const uint8_t colorPixel = pSourcePixels[sourceIndex];

                const uint8_t* finalPixel = (colorPixel*4 + (palette));

                const uint32_t destinationIndex = y * width + x;
                
                pPixels[destinationIndex].b = finalPixel[0];
                pPixels[destinationIndex].g = finalPixel[1];
                pPixels[destinationIndex].r = finalPixel[2];
                pPixels[destinationIndex].a = 255;
            }
        }
    }
        

    jv::util::Vec2 pxSize = {static_cast<float>(width), static_cast<float>(height)};

    jv::gpu::Texture* pTexture = jv::gpu::CreateTexture(imageName, pxSize, pixels);
    return {pTexture, pxSize};
}
