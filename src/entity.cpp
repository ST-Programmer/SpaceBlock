#include "entity.h"
#include "game.h"
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <iostream>

Entity::Entity(SDL_Renderer* rend, const std::string& texturePath,
               const glm::vec4& textureBoundingBox, const glm::vec2& isometricPos,
               const glm::vec2& scale)
    : m_IsometricPos(isometricPos)
{
    SDL_Surface* surf = SDL_LoadSurface(texturePath.c_str());

    if (!surf)
    {
        std::cerr << "Failed To Create Texture From: " << texturePath << std::endl;
        std::cerr << "Error Details: " << SDL_GetError() << std::endl;
        exit(-1);
    }

    m_Texture = SDL_CreateTextureFromSurface(rend, surf);

    if (!m_Texture)
    {
        std::cerr << "Failed To Create Texture From: " << texturePath << std::endl;
        std::cerr << "Error Details: " << SDL_GetError() << std::endl;
        exit(-1);
    }

    SDL_SetTextureScaleMode(m_Texture, SDL_SCALEMODE_PIXELART);

    // GLM::Vec4 --> {x, y, z, w} --> {x, y, w, h} --> w = z, h = w
    float sizeX = textureBoundingBox.z; // Width
    float sizeY = textureBoundingBox.w; // Height

    glm::vec2 pos = Game::TranslateIsoToScreen(m_IsometricPos);

    m_SrcRect = {textureBoundingBox.x, textureBoundingBox.y, sizeX, sizeY};
    m_PosRect = {pos.x, pos.y, sizeX * scale.x, sizeY * scale.y};

    SDL_DestroySurface(surf);
}

Entity::~Entity() { SDL_DestroyTexture(m_Texture); }

Entity::Entity(Entity&& other) noexcept
{
    m_SrcRect = other.m_SrcRect;
    m_PosRect = other.m_PosRect;
    m_Texture = other.m_Texture;

    other.m_SrcRect = {0};
    other.m_PosRect = {0};
    other.m_Texture = nullptr;
}

Entity& Entity::operator=(Entity&& other) noexcept
{
    if (this != &other)
    {
        SDL_DestroyTexture(m_Texture);

        m_SrcRect = other.m_SrcRect;
        m_PosRect = other.m_PosRect;
        m_Texture = other.m_Texture;

        other.m_SrcRect = {0};
        other.m_PosRect = {0};
        other.m_Texture = nullptr;
    }

    return *this;
}

void Entity::Update(float dt) {}
