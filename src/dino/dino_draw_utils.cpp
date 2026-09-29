#include <dino/dino_draw_utils.h>
#include <dino/dino_main.h>
#include <math.h>
#include <string.h>

// Informations sur le format BMP:
// https://www.ece.ualberta.ca/~elliott/ee552/studentAppNotes/2003_w/misc/bmp_file_format/bmp_file_format.htm

jv::gpu::Texture* dino::LoadImageAsset(std::string_view imageName)
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
    uint32_t pxWidth = *(uint32_t*)(pFile + 0x0012);
    uint32_t pxHeight = *(uint32_t*)(pFile + 0x0016);

    uint16_t bitsPerPixel = *(uint16_t*)(pFile + 0x001C);
    if (bitsPerPixel != 8 && bitsPerPixel != 24)
        return {};
    unsigned char* pColorTable = nullptr;
    if (bitsPerPixel == 8)
        pColorTable = pFile + 0x0036;

    // Destination
    std::vector<jv::util::Color> pixels;
    pixels.resize(pxWidth * pxHeight);

    unsigned char const* pPixel = pFile + dataOffset;

    for (size_t y = 0; y < pxHeight; ++y)
    {
        // L'ordre des lignes est inversé.
        jv::util::Color* pDst = pixels.data() + (pxHeight - y - 1) * pxWidth;

        for (size_t x = 0; x < pxWidth; ++x)
        {
            unsigned char const* pRGB;
            if (bitsPerPixel == 8)
            {
                uint8_t index = pPixel[0];
                pRGB = pColorTable + 4 * index;
                pPixel += 1;
            }
            else
            {
                pRGB = pPixel;
                pPixel += 3;
            }
            pDst->b = pRGB[0];
            pDst->g = pRGB[1];
            pDst->r = pRGB[2];
            if (pDst->r == 255 && pDst->g == 0 && pDst->b == 255)
                pDst->a = 0;
            else
                pDst->a = 255;
            pDst += 1;
        }
    }

    Vec2 pxSize = {(float)pxWidth, (float)pxHeight};
    return jv::gpu::CreateTexture(imageName, pxSize, pixels);
}

jv::util::Vec2 dino::GenVertices_Text(std::vector<jv::gpu::Vertex>& out, std::string_view text,
    jv::util::Color color, jv::util::Color colorBackground, Vec2 pos)
{
    // Il y a au maximum un quad = deux triangles pour chaque octet du texte, et un quad pour le
    // fond.
    size_t oldSize = out.size();
    out.reserve(oldSize + 6 + 6 * text.size());

    // On laisse les 6 premiers vertices pour le fond, pour qu'il soit dessiné en premier.
    // On donnera les positions à la fin, car on ne connait pas encore la taille du texte.
    if (colorBackground.a > 0)
        out.resize(oldSize + 6);

    // Dimensions en pixels des lettres dans la texture.
    constexpr float QUAD_WIDTH = 6;
    constexpr float QUAD_HEIGHT = 12;

    // Pour suivre la position des caractères au fur et à mesure.
    float x = 0;
    float y = 0;

    // Pour suivre la taille totale du texte.
    float xMax = 0;

    for (unsigned char c : text)
    {
        if (c == '\n')
        {
            // Retour à la ligne
            xMax = std::max(xMax, x);
            x = 0;
            y += QUAD_HEIGHT;
        }
        else if (c <= 31)
        {
            // Caractères de contrôle, on ne les gère pas = on les ignore = on ne fait rien.
        }
        else
        {
            // Caractères étendus, en dehors de l'ASCII.
            // On ne les gère pas = on affiche un '?' en tant qu'erreur.
            if (c >= 128)
                c = '?';

            int32_t xIdx = c % 16;
            int32_t yIdx = c / 16;
            float u = xIdx * QUAD_WIDTH;
            float v = yIdx * QUAD_HEIGHT;

            Vec2 posA = {x + pos.x, y + pos.y};
            Vec2 posB = {x + pos.x + QUAD_WIDTH, y + pos.y};
            Vec2 posC = {x + pos.x, y + pos.y + QUAD_HEIGHT};
            Vec2 posD = {x + pos.x + QUAD_WIDTH, y + pos.y + QUAD_HEIGHT};
            out.emplace_back(posA, Vec2{u, v}, color);
            out.emplace_back(posB, Vec2{u + QUAD_WIDTH, v}, color);
            out.emplace_back(posC, Vec2{u, v + QUAD_HEIGHT}, color);
            out.emplace_back(posB, Vec2{u + QUAD_WIDTH, v}, color);
            out.emplace_back(posC, Vec2{u, v + QUAD_HEIGHT}, color);
            out.emplace_back(posD, Vec2{u + QUAD_WIDTH, v + QUAD_HEIGHT}, color);

            x += QUAD_WIDTH;
        }
    }
    xMax = std::max(xMax, x);

    float width = static_cast<float>(xMax);
    float height = static_cast<float>(y + QUAD_HEIGHT);

    if (colorBackground.a > 0)
    {
        // Les 6 premiers points ont été laissés libres pour le fond.
        out[oldSize + 0] = {pos, Vec2{0, 0}, colorBackground};
        out[oldSize + 1] = {{pos.x + width, pos.y}, Vec2{0, 0}, colorBackground};
        out[oldSize + 2] = {{pos.x, pos.y + height}, Vec2{0, 0}, colorBackground};
        out[oldSize + 3] = {{pos.x + width, pos.y}, Vec2{0, 0}, colorBackground};
        out[oldSize + 4] = {{pos.x, pos.y + height}, Vec2{0, 0}, colorBackground};
        out[oldSize + 5] = {{pos.x + width, pos.y + height}, Vec2{0, 0}, colorBackground};
    }
    return {width, height};
}

void dino::GenVertices_Polyline(std::vector<jv::gpu::Vertex>& out, std::vector<Vec2> const& points,
    float width, jv::util::Color color)
{
    // En bonus, pour plus d'infos sur l'algorithme, voir :
    // https://jvernay.fr/en/blog/points-triangulation/

    // On ne supporte pas les couleurs transparentes, désolé.
    if (color.a == 0)
        return; // Invisible.
    color.a = 255;

    if (points.size() <= 1)
        return; // Degenerate case: cannot trace a line from 0 or 1 point.

    // Only compute these constants once.
    float halfWidth = width / 2;

    size_t idxPoint = 0;

    // First point.
    Vec2 A = points[idxPoint];
    idxPoint += 1;

    Vec2 B;
    float lenAB = 0;
    while (idxPoint < points.size() && lenAB == 0)
    {
        // Get second point, such that A and B are distinct.
        B = points[idxPoint];
        lenAB = hypotf(B.x - A.x, B.y - A.y);
        idxPoint += 1;
    }
    if (lenAB == 0)
        return; // Degenerate case: all points are identical.

    for (; idxPoint <= points.size(); idxPoint += 1)
    {

        // Get next point C such that B and C are distinct.
        Vec2 C;
        float lenBC;
        if (idxPoint < points.size())
        {
            C = points[idxPoint];
            lenBC = hypotf(C.x - B.x, C.y - B.y);
        }
        else
        {
            // [AB] is the last segment to render, no join is needed at the end.
            // Making C = A such that (AB) and (BC) are aligned, thus no join will be generated.
            C = A;
            lenBC = lenAB;
        }
        if (lenBC == 0)
            continue; // Empty segment, do nothing.

        // Compute quad for segment AB.

        float xAB = B.x - A.x, yAB = B.y - A.y;
        float xAA1 = halfWidth / lenAB * -yAB;
        float yAA1 = halfWidth / lenAB * xAB;
        Vec2 A1{A.x + xAA1, A.y + yAA1};
        Vec2 A2{A.x - xAA1, A.y - yAA1};
        Vec2 A1p{B.x + xAA1, B.y + yAA1};
        Vec2 A2p{B.x - xAA1, B.y - yAA1};

        // Encode quad for segment AB as two triangles.
        out.emplace_back(A1, Vec2{}, color);
        out.emplace_back(A2, Vec2{}, color);
        out.emplace_back(A1p, Vec2{}, color);
        out.emplace_back(A2, Vec2{}, color);
        out.emplace_back(A1p, Vec2{}, color);
        out.emplace_back(A2p, Vec2{}, color);

        // Determine the ABC angle's orientation.

        float xBC = C.x - B.x, yBC = C.y - B.y;
        float zAB_BC = xAB * yBC - yAB * xBC;

        // If zAB_BC == 0, A B C are aligned and no join is needed.
        if (zAB_BC != 0)
        {
            // Compute the endpoints of the next segment.

            float xBB1 = halfWidth / lenBC * -yBC;
            float yBB1 = halfWidth / lenBC * xBC;
            Vec2 B1{B.x + xBB1, B.y + yBB1};
            Vec2 B2{B.x - xBB1, B.y - yBB1};

            // Generate Bevel join triangle.
            if (zAB_BC < 0)
            {
                out.emplace_back(B, Vec2{}, color);
                out.emplace_back(A1p, Vec2{}, color);
                out.emplace_back(B1, Vec2{}, color);
            }
            else
            {
                out.emplace_back(B, Vec2{}, color);
                out.emplace_back(A2p, Vec2{}, color);
                out.emplace_back(B2, Vec2{}, color);
            }
        }
        // Prepare for next segment.
        A = B;
        B = C;
        lenAB = lenBC;
    }
}

void dino::GenVertices_Rect(std::vector<jv::gpu::Vertex>& vertices, Vec2 topLeft, Vec2 size,
    Vec2 topLeftUV, jv::util::Color color)
{
    float x = topLeft.x;
    float y = topLeft.y;
    float xw = x + size.x;
    float yh = y + size.y;
    float u = topLeftUV.x;
    float v = topLeftUV.y;
    float uw = topLeftUV.x + size.x;
    float vh = topLeftUV.y + size.y;
    vertices.emplace_back(Vec2{x, y}, Vec2{u, v}, color);
    vertices.emplace_back(Vec2{xw, y}, Vec2{uw, v}, color);
    vertices.emplace_back(Vec2{x, yh}, Vec2{u, vh}, color);
    vertices.emplace_back(Vec2{xw, y}, Vec2{uw, v}, color);
    vertices.emplace_back(Vec2{x, yh}, Vec2{u, vh}, color);
    vertices.emplace_back(Vec2{xw, yh}, Vec2{uw, vh}, color);
}