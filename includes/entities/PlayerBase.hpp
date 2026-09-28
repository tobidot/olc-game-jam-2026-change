#pragma once
#include "enums/Enums.hpp"
#include "plans/BasePlan.hpp"

namespace entity
{

class PlayerBase : public Entity
{
public:
    enums::CharacterType type;

public:
    explicit PlayerBase(enums::CharacterType type);
    PlayerBase(const PlayerBase &cpy) = delete;
    PlayerBase(PlayerBase &&cpy) = delete;
    ~PlayerBase() override = default;
    PlayerBase &operator=(const PlayerBase &other) = delete;
    PlayerBase &operator=(PlayerBase &&other) = delete;
};

} // namespace entity