#pragma once

#include "entity.h"

enum class PlantType;

class Plant : public Entity
{
public:
    Plant(PlantType type, SDL_Renderer* rend, const std::string& texturePath,
          const glm::vec4& textureBoundingBox, const glm::vec2& isometricPos,
          const glm::vec2& scale = {1.0f, 1.0f});

    inline const PlantType& Type() const { return m_Type; }
    inline void ChangeType(PlantType type) { m_Type = type; }
    void Update(float dt);

private:
    PlantType m_Type;
};

enum class PlantType
{
    BASIC
};
