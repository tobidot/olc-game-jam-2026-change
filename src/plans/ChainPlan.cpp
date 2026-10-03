#include "plans/ChainPlan.hpp"

using namespace plan;

ChainPlan::ChainPlan(std::vector<std::unique_ptr<BasePlan>> plans) : plans(std::move(plans))
{
}

void ChainPlan::Start(state::App &state, entity::Entity &entity)
{
    BasePlan::Start(state, entity);
    if (index < plans.size())
    {
        plans.at(index)->Start(state, entity);
    }
}

void ChainPlan::Update(state::App &state, entity::Entity &entity, float elapsed_time)
{
    BasePlan::Update(state, entity, elapsed_time);

    if (index >= plans.size())
    {
        is_finished = true;
        return;
    }
    const auto &current_plan = plans.at(index);
    if (current_plan->is_finished)
    {
        index++;
        if (index < plans.size())
        {
            const auto &next_plan = plans.at(index);
            next_plan->Start(state, entity);
            return;
        }
        is_finished = true;

        return;
    }
    current_plan->Update(state, entity, elapsed_time);
}