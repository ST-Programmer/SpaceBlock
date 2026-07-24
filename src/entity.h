#pragma once

#include <SDL3/SDL.h>
#include <glm/glm.hpp>

// Interface/Abstract
class Entity
{
public:
    Entity(SDL_Renderer* rend, const std::string& texturePath, const glm::vec4& textureBoundingBox,
           const glm::vec2& isometricPos, const glm::vec2& scale = {1.0f, 1.0f});

    virtual ~Entity();

    Entity(const Entity& other) = delete;
    Entity& operator=(const Entity& other) = delete;
    Entity(Entity&& other) noexcept;
    Entity& operator=(Entity&& other) noexcept;

    virtual void Update(float dt) = 0;

    inline SDL_Texture* Texture() const { return m_Texture; }
    inline const SDL_FRect& SrcRect() const { return m_SrcRect; }
    inline const SDL_FRect& PosRect() const { return m_PosRect; }

private:
    SDL_FRect m_SrcRect;
    SDL_Texture* m_Texture;

protected:
    SDL_FRect m_PosRect;
    glm::vec2 m_IsometricPos;
};
