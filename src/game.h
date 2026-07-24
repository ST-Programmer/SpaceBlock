#pragma once

#include "entity.h"
#include "plant.h"
#include "player.h"
#include "tile.h"
#include <SDL3/SDL.h>
#include <cstdint>
#include <memory>
#include <vector>

class Game
{
public:
    Game(const std::string& windowTitle, int32_t windowWidth, int32_t windowHeight);
    ~Game();

    Game(const Game& other) = delete;
    Game& operator=(const Game& other) = delete;
    Game(Game&& other) = delete;
    Game& operator=(Game&& other) = delete;

    void CreateTerrain();
    static void AddPlant(Plant&& plant);
    static void AddTile(Tile&& tile);
    void DrawEntities() const;
    void DrawTerrain() const;
    void DrawHoveredTile() const;
    void UpdateHoveredTile();
    void UpdateTilePositions(); // On Window Resize

    void HandleInput();
    void Loop();
    void Render() const;
    void Update();
    void ClearScreen() const;

    static const Tile* GetTileAt(const glm::vec2& screenPos);
    static glm::vec2 TranslateScreenToIso(const glm::vec2& v);
    static glm::vec2 TranslateIsoToScreen(const glm::vec2& v);

public:
    constexpr static int32_t s_TileWidth = 64;
    constexpr static int32_t s_TileHeight = 64;
    constexpr static int32_t s_MapWidth = 64;
    constexpr static int32_t s_MapHeight = 64;
    constexpr static int32_t s_MapElevation = 8;

private:
    void Destroy();

private:
    inline static int32_t s_WindowWidth = 0;
    inline static int32_t s_WindowHeight = 0;
    inline static float s_NoiseFrequency = 0.07;

    inline static glm::vec4 s_BgColor{8, 11, 20, 255};

    inline static const glm::mat2 s_IsoToScreenTransform = {
        glm::vec2(1.0f * s_TileWidth / 2, 0.5f * s_TileHeight / 2),
        glm::vec2(-1.0f * s_TileWidth / 2, 0.5f * s_TileHeight / 2)};
    inline static const glm::mat2 s_ScreenToIsoTransform = glm::inverse(s_IsoToScreenTransform);

    inline static std::vector<Plant> s_Plants;
    inline static std::vector<Tile> s_Tiles;
    std::string m_AtlasPath = "res/isometricatlas.png";

    SDL_Window* m_Window = nullptr;
    SDL_Renderer* m_Renderer = nullptr;
    SDL_Event m_Event;

    std::unique_ptr<Player> m_Player;
    const Tile* m_HoveredTile = nullptr;

    uint64_t m_LastCounter = 0;
    float m_DeltaTime = 0;
    float mousePosX = 0;
    float mousePosY = 0;
    bool m_Running = true;
};
