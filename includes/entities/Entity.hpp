#pragma once
#include "core/olcPixelGameEngine3.h"

namespace entity
{

class Entity
{
public:
    float health = 100.0f;
    float max_health = 100.0f;
    core::Vector position;
    core::Shape shape;
    core::Animator animator;

public:
    void Update(float elapsed_time);
};

} // namespace entity