#include "player.h"
#include "game.h"
#include "tile.h"
#include <SDL3/SDL.h>
#include <iostream>

Player::Player(SDL_Renderer* rend, const std::string& texturePath,
               const glm::vec4& textureBoundingBox, const glm::vec2& isometricPos,
               const glm::vec2& scale)
    : Entity(rend, texturePath, textureBoundingBox, isometricPos, scale)
{
}

void Player::Update(float dt)
{
    const bool* keyState = SDL_GetKeyboardState(nullptr);
    glm::vec2 move(0.0f);

    if (keyState[SDL_SCANCODE_W])
        move += glm::vec2(0.0f, -1.0f);
    if (keyState[SDL_SCANCODE_S])
        move += glm::vec2(0.0f, 1.0f);
    if (keyState[SDL_SCANCODE_A])
        move += glm::vec2(-1.0f, 0.0f);
    if (keyState[SDL_SCANCODE_D])
        move += glm::vec2(1.0f, 0.0f);
    if (glm::length(move) > 0.0f)
        move = glm::normalize(move);

    m_IsometricPos += move * m_WalkSpeed * dt;

    glm::vec2 anchor = Game::TranslateIsoToScreen(m_IsometricPos);
    float groundX = anchor.x + Game::s_TileWidth / 2.0f;
    float groundY = anchor.y + Game::s_TileHeight;

    m_PosRect.x = groundX - m_PosRect.w / 2.0f;
    m_PosRect.y = groundY - m_PosRect.h;
}
