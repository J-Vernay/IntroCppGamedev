#pragma once

#include <map>
#include <string>
#include <jv/jv.h>

namespace dino
{
class AssetsHolder {
public:
    // Singleton principle
    static AssetsHolder& getInstance()
    {
        static AssetsHolder obj;
        return obj;
    }

    std::map<std::string, jv::gpu::Texture*> g_Textures;

	void LoadTextures();

    ~AssetsHolder();

private:
};
} // namespace dino