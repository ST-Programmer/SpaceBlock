#pragma once
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <memory>
#include <string>

enum class TileHover;
enum class TileType;
struct SDLTextureDeleter
{
    void operator()(SDL_Texture* texture) const
    {
        if (texture)
            SDL_DestroyTexture(texture);
    }
};

class Tile
{
public:
    Tile(TileType type, const glm::vec2& isoPos, int32_t elevationLevel = 0);

    // Run Once (Per Atlas)
    static void CreateTileTextures(SDL_Renderer* rend, const std::string& atlasPath);

    inline void ChangeTileType(TileType type) { m_Type = type; }
    inline void SetScreenPos(const glm::vec2& pos)
    {
        m_PosRect.x = pos.x;
        m_PosRect.y = pos.y;
    }

    inline SDL_Texture* Texture() const { return textureAtlas.get(); }
    inline const SDL_FRect& PosRect() const { return m_PosRect; }
    inline const glm::vec2& IsoPos() const { return m_IsoPos; }
    const SDL_FRect& SrcRect() const;
    const SDL_FRect& HoverRect() const;

    inline const SDL_FRect& RockTextureRect() const { return rockRect; }
    inline const SDL_FRect& GrassTextureRect() const { return grassRect; }
    inline const SDL_FRect& ClayTextureRect() const { return clayRect; }
    inline const SDL_FRect& DirtTextureRect() const { return dirtRect; }
    inline const SDL_FRect& SandTextureRect() const { return sandRect; }
    inline const SDL_FRect& CrimsonTextureRect() const { return crimsonRect; }
    inline int32_t Elevation() const { return m_Elevation; }
    inline void SetElevation(int32_t e) { m_Elevation = e; }
    inline static int32_t ElevationStep() { return s_ElevationStep; }

private:
    inline constexpr static SDL_FRect MakeRect(int col, int row, uint32_t offsetX)
    {
        return {static_cast<float>(offsetX + col * s_TileWidth),
                static_cast<float>(row * s_TileHeight), s_TileWidth, s_TileHeight};
    }

private:
    constexpr static uint32_t s_TileWidth = 64;
    constexpr static uint32_t s_TileHeight = 64;
    constexpr static uint32_t s_HoverOffsetX = 448;
    constexpr static float s_ElevationStep = s_TileHeight / 2.0;

    inline static std::unique_ptr<SDL_Texture, SDLTextureDeleter> textureAtlas{};

    inline static SDL_FRect rockRect = MakeRect(0, 0, 0);
    inline static SDL_FRect grassRect = MakeRect(1, 0, 0);
    inline static SDL_FRect clayRect = MakeRect(2, 0, 0);
    inline static SDL_FRect dirtRect = MakeRect(0, 1, 0);
    inline static SDL_FRect sandRect = MakeRect(1, 1, 0);
    inline static SDL_FRect crimsonRect = MakeRect(2, 1, 0);

    inline static SDL_FRect rockHoverRect = MakeRect(0, 0, s_HoverOffsetX);
    inline static SDL_FRect grassHoverRect = MakeRect(1, 0, s_HoverOffsetX);
    inline static SDL_FRect clayHoverRect = MakeRect(2, 0, s_HoverOffsetX);
    inline static SDL_FRect dirtHoverRect = MakeRect(0, 1, s_HoverOffsetX);
    inline static SDL_FRect sandHoverRect = MakeRect(1, 1, s_HoverOffsetX);
    inline static SDL_FRect crimsonHoverRect = MakeRect(2, 1, s_HoverOffsetX);

    TileType m_Type;
    glm::vec2 m_IsoPos;
    SDL_FRect m_PosRect;
    int32_t m_Elevation = 0;
};

enum class TileType
{
    ROCK,
    GRASS,
    CLAY,
    DIRT,
    SAND,
    CRIMSON
};

enum class TileHover
{
    ROCKHOVER,
    GRASSHOVER,
    CLAYHOVER,
    DIRTHOVER,
    SANDHOVER,
    CRIMSONHOVER
};
