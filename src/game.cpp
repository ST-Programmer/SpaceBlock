#include "game.h"
#include "FastNoiseLite/Cpp/FastNoiseLite.h"
#include "player.h"
#include "tile.h"
#include <cstdint>
#include <glm/glm.hpp>

Game::Game(const std::string& windowTitle, int32_t winWidth, int32_t winHeight)
{
    SDL_Init(SDL_INIT_VIDEO);

    s_WindowWidth = winWidth;
    s_WindowHeight = winHeight;

    m_Window =
        SDL_CreateWindow(windowTitle.data(), s_WindowWidth, s_WindowHeight, SDL_WINDOW_RESIZABLE);
    m_Renderer = SDL_CreateRenderer(m_Window, NULL);
    SDL_SetRenderVSync(m_Renderer, true);

    SDL_RaiseWindow(m_Window);
    SDL_GetMouseState(&mousePosX, &mousePosY);

    m_LastCounter = SDL_GetPerformanceCounter();

    Tile::CreateTileTextures(m_Renderer, m_AtlasPath);
    CreateTerrain();

    m_Player = std::make_unique<Player>(m_Renderer, m_AtlasPath, glm::vec4(320, 0, 96, 96),
                                        glm::vec2(0, 0));
}

Game::~Game() { Destroy(); }

void Game::CreateTerrain()
{
    s_Tiles.reserve(s_MapWidth * s_MapHeight * s_MapElevation);

    FastNoiseLite noise;
    noise.SetSeed(1337);
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise.SetFrequency(s_NoiseFrequency);

    float heightScale = s_MapElevation;

    for (int32_t y = -s_MapHeight / 2; y < s_MapHeight / 2; y++)
    {
        for (int32_t x = -s_MapWidth / 2; x < s_MapWidth / 2; x++)
        {
            float n = noise.GetNoise(static_cast<float>(x), static_cast<float>(y));

            n = 0.5 * (n + 1); // [1, 1] --> [0, 1]

            uint32_t elevation = static_cast<uint32_t>(std::round(n * heightScale));
            elevation = std::min(elevation, static_cast<uint32_t>(s_MapElevation));

            for (int32_t z = 0; z <= elevation; z++)
            {
                TileType type = TileType::ROCK;

                if (z == 5)
                {
                    type = TileType::DIRT;
                }

                if (z == 6)
                {
                    type = TileType::GRASS;
                }

                s_Tiles.emplace_back(type, glm::vec2(x, y), z);
            }
        }
    }
}

void Game::AddPlant(Plant&& plant) { s_Plants.push_back(std::move(plant)); }
void Game::AddTile(Tile&& tile) { s_Tiles.push_back(std::move(tile)); }

glm::vec2 Game::TranslateIsoToScreen(const glm::vec2& v)
{
    glm::vec2 newVector = Game::s_IsoToScreenTransform * v;
    newVector.x = newVector.x + s_WindowWidth / 2.0f - s_TileWidth / 2.0f;
    newVector.y = newVector.y + s_WindowHeight / 2.0f - s_TileHeight / 2.0f;
    return newVector;
}

glm::vec2 Game::TranslateScreenToIso(const glm::vec2& v)
{
    glm::vec2 newVector = v;
    newVector.x -= s_WindowWidth / 2.0f - s_TileWidth / 2.0f;
    newVector.y -= s_WindowHeight / 2.0f - s_TileHeight / 2.0f;

    return Game::s_ScreenToIsoTransform * newVector;
}

const Tile* Game::GetTileAt(const glm::vec2& screenPos)
{
    for (int z = s_MapElevation; z >= 0; z--)
    {
        glm::vec2 adjustedScreenPos = screenPos;
        adjustedScreenPos.y += z * Tile::ElevationStep();

        glm::vec2 isoPos = TranslateScreenToIso(adjustedScreenPos);
        int32_t x = static_cast<int32_t>(std::round(isoPos.x)) - 1; // Tile Offset
        int32_t y = static_cast<int32_t>(std::round(isoPos.y));

        for (const Tile& tile : s_Tiles)
        {
            if (tile.IsoPos().x == x && tile.IsoPos().y == y && tile.Elevation() == z)
            {
                return &tile;
            }
        }
    }

    return nullptr;
}
void Game::HandleInput()
{
    while (SDL_PollEvent(&m_Event))
    {
        switch (m_Event.type)
        {
        case SDL_EVENT_KEY_DOWN:
            if (m_Event.key.key == SDLK_ESCAPE)
            {
                m_Running = false;
            }
            break;

        case SDL_EVENT_MOUSE_MOTION:
            SDL_GetMouseState(&mousePosX, &mousePosY);
            break;

        case SDL_EVENT_WINDOW_RESIZED:
            SDL_GetWindowSize(m_Window, &s_WindowWidth, &s_WindowHeight);
            UpdateTilePositions();
            SDL_GetMouseState(&mousePosX, &mousePosY);
            break;

        case SDL_EVENT_QUIT:
            m_Running = false;
            break;

        default:
            break;
        }
    }
}

void Game::ClearScreen() const
{
    SDL_SetRenderDrawColor(m_Renderer, s_BgColor.x, s_BgColor.y, s_BgColor.z, s_BgColor.w);
    SDL_RenderClear(m_Renderer);
}

void Game::Update()
{
    SDL_GetWindowSize(m_Window, &s_WindowWidth, &s_WindowHeight);
    SDL_GetMouseState(&mousePosX, &mousePosY);

    const uint64_t counterFreq = SDL_GetPerformanceFrequency();
    uint64_t now = SDL_GetPerformanceCounter();
    m_DeltaTime = static_cast<float>(now - m_LastCounter) / static_cast<float>(counterFreq);
    if (m_DeltaTime > 1.0f / 30.0f)
        m_DeltaTime = 1.0f / 30.0f;

    for (auto& plant : s_Plants)
    {
        plant.Update(m_DeltaTime);
    }

    m_Player->Update(m_DeltaTime);

    UpdateHoveredTile();

    m_LastCounter = now;
}

void Game::DrawEntities() const
{
    for (const auto& plant : s_Plants)
    {
        SDL_RenderTexture(m_Renderer, plant.Texture(), &plant.SrcRect(), &plant.PosRect());
    }

    SDL_RenderTexture(m_Renderer, m_Player->Texture(), &m_Player->SrcRect(), &m_Player->PosRect());
}

void Game::DrawTerrain() const
{
    for (const Tile& tile : s_Tiles)
    {
        SDL_RenderTexture(m_Renderer, tile.Texture(), &tile.SrcRect(), &tile.PosRect());
    }
}

void Game::UpdateHoveredTile() { m_HoveredTile = GetTileAt({mousePosX, mousePosY}); }

void Game::DrawHoveredTile() const
{
    if (m_HoveredTile)
    {
        SDL_RenderTexture(m_Renderer, m_HoveredTile->Texture(), &m_HoveredTile->HoverRect(),
                          &m_HoveredTile->PosRect());
    }
}

void Game::UpdateTilePositions()
{
    for (Tile& tile : s_Tiles)
    {
        glm::vec2 pos = TranslateIsoToScreen(tile.IsoPos());
        pos.y -= tile.Elevation() * Tile::ElevationStep();
        tile.SetScreenPos(pos);
    }
}

void Game::Render() const
{
    DrawTerrain();
    DrawHoveredTile();
    DrawEntities();

    SDL_RenderPresent(m_Renderer);
}

void Game::Loop()
{
    while (m_Running)
    {
        HandleInput();
        ClearScreen();
        Update();
        Render();
    }
}

void Game::Destroy()
{
    s_Tiles.clear();
    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Window);
    SDL_Quit();
}
