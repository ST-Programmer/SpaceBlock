#include "tile.h"
#include "game.h"
#include <SDL3/SDL.h>
#include <iostream>

Tile::Tile(TileType type, const glm::vec2& isoPos, int32_t elevationLevel)
    : m_Type(type), m_IsoPos(isoPos), m_Elevation(elevationLevel)
{

    glm::vec2 pos = Game::TranslateIsoToScreen(isoPos);
    pos.y -= m_Elevation * s_ElevationStep;
    m_PosRect = {pos.x, pos.y, s_TileWidth, s_TileHeight};

    if (!textureAtlas)
    {
        std::cerr << "Failed To Initialized Texture Atlas --> "
                  << "CreateTileTextures() Must Be Invoked Before Tile Construction)" << std::endl;
    }
}

const SDL_FRect& Tile::SrcRect() const
{
    switch (m_Type)
    {
    case TileType::ROCK:
        return rockRect;
        break;
    case TileType::GRASS:
        return grassRect;
        break;
    case TileType::CLAY:
        return clayRect;
        break;
    case TileType::DIRT:
        return dirtRect;
        break;
    case TileType::SAND:
        return sandRect;
        break;
    case TileType::CRIMSON:
        return crimsonRect;
        break;
    }
}

const SDL_FRect& Tile::HoverRect() const
{

    switch (m_Type)
    {
    case TileType::ROCK:
        return rockHoverRect;
        break;
    case TileType::GRASS:
        return grassHoverRect;
        break;
    case TileType::CLAY:
        return clayHoverRect;
        break;
    case TileType::DIRT:
        return dirtHoverRect;
        break;
    case TileType::SAND:
        return sandHoverRect;
        break;
    case TileType::CRIMSON:
        return crimsonHoverRect;
        break;
    }
}

void Tile::CreateTileTextures(SDL_Renderer* rend, const std::string& atlasPath)
{
    SDL_Surface* surf = SDL_LoadSurface(atlasPath.c_str());
    if (!surf)
    {
        std::cerr << "Failed To Load Surface From: " << atlasPath << "\n";
        std::cerr << "Error Details: " << SDL_GetError() << "\n";
        exit(-1);
    }

    textureAtlas.reset(SDL_CreateTextureFromSurface(rend, surf));
    SDL_DestroySurface(surf);

    if (!textureAtlas)
    {
        std::cerr << "Failed To Create Texture From: " << atlasPath << "\n";
        std::cerr << "Error Details: " << SDL_GetError() << "\n";
        exit(-1);
    }

    SDL_SetTextureScaleMode(textureAtlas.get(), SDL_SCALEMODE_PIXELART);
}
