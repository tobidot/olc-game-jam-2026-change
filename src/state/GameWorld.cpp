#include "state/GameWorld.hpp"

#include "entities/Entity.hpp"
#include "entities/EntityHandle.hpp"

#include <memory>

using namespace state;

const std::shared_ptr<entity::EntityHandle> GameWorld::FindEntityHandle(size_t id)
{
    auto iterator = std::ranges::find_if(
        entities,
        [id](const std::shared_ptr<entity::EntityHandle> &handle) { return handle->ref->id == id; }
    );

    if (iterator != entities.end())
    {
        return *iterator;
    }

    return std::shared_ptr<entity::EntityHandle>(nullptr);
}

size_t GameWorld::GetNextEntityID() noexcept
{
    return next_entity_id++;
}