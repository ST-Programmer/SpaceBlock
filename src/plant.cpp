#include "plant.h"

Plant::Plant(PlantType type, SDL_Renderer* rend, const std::string& texturePath,
             const glm::vec4& textureBoundingBox, const glm::vec2& isometricPos,
             const glm::vec2& scale)
    : Entity(rend, texturePath, textureBoundingBox, isometricPos, scale), m_Type(type)
{
}

void Plant::Update(float dt) {}
