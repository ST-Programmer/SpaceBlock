#pragma once

#include "entity.h"
#include "glm/glm.hpp"

class Player : public Entity
{
public:
    Player(SDL_Renderer* rend, const std::string& texturePath, const glm::vec4& textureBoundingBox,
           const glm::vec2& isometricPos, const glm::vec2& scale = {1.0f, 1.0f});

    void Update(float dt) override;

private:
    float m_WalkSpeed = 9;
};
