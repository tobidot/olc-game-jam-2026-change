#include "entities/Entity.hpp"
#include "enums/Enums.hpp"
#include "plans/Callback.hpp"
#include "state/App.hpp"

using namespace plan;

Callback::Callback(const std::function<void()> &callback) : callback(callback)
{
    name = "Callback";
}

void Callback::Start(state::App &state, entity::Entity &entity)
{
    callback();
    is_finished = true;
}

void Callback::Update(state::App &state, entity::Entity &entity, float elapsed_time)
{
    BasePlan::Update(state, entity, elapsed_time);
}