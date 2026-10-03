#include "plans/BasePlan.hpp"

#include <tuple>

using namespace plan;

void BasePlan::Start(state::App &state, entity::Entity &entity)
{
}

void BasePlan::Update(state::App &state, entity::Entity &entity, float elapsed_time)
{
}

bool BasePlan::IsInTimeWindow(std::pair<float, float> window, float now, float elapsed_time) const
{
    const auto time_after_update = now + elapsed_time;
    return (now < window.second && time_after_update > window.first);
}