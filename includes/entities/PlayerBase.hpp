#pragma once
#include "plans/BasePlan.hpp"

namespace entity
{

class PlayerBase : public Entity
{
public:
    PlayerBase() = default;
    PlayerBase(const PlayerBase &cpy) = delete;
    PlayerBase(PlayerBase &&cpy) = delete;
    ~PlayerBase() override = default;
    PlayerBase &operator=(const PlayerBase &other) = delete;
    PlayerBase &operator=(PlayerBase &&other) = delete;
};

} // namespace entity