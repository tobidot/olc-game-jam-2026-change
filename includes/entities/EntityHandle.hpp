#pragma once
#include <memory>

namespace entity
{
class Entity;

using EntityRef = std::shared_ptr<Entity>;

struct EntityHandle
{
public:
    EntityRef ref;

public:
    EntityHandle() = delete;
    explicit EntityHandle(EntityRef ref) : ref(std::move(ref)) {};
    EntityHandle(const EntityHandle &cpy) = default;
    EntityHandle(EntityHandle &&move) noexcept : ref(std::move(move.ref)) {};
    ~EntityHandle() = default;

public:
    EntityHandle &operator=(const EntityHandle &cpy)
    {
        if (&cpy == this)
        {
            return *this;
        }
        ref = cpy.ref;
        return *this;
    }
    EntityHandle &operator=(EntityHandle &&cpy) noexcept
    {
        if (&cpy == this)
        {
            return *this;
        }
        ref = std::move(cpy.ref);
        return *this;
    }
};

} // namespace entity