#pragma once
#include "ComponentStateIdleWalk.h"
#include <Game/Component/ComponentStatePickUp.h>

void ComponentStatePickUp::Init()
{
    __super::Init();

    SetName<Component>("State PickUp");

    const float3 up_len = {0.0f, 5.0f, 0.0f};
    float3       vec    = {0.0f, 1.0f, 0.0f};
    if(auto model = GetOwner()->GetComponent<ComponentModel>()) {
        constexpr float front_len = 5.0f;

        vec = -model->GetWorldVectorAxisZ() * front_len;

        model->PlayAnimation("PickUp");
    }
}

void ComponentStatePickUp::Update()
{
    __super::Update();

    // 理想的なものに切り替えた
    ChangeStateWhenAnimEnd<ComponentStateIdleWalk>();
}

void ComponentStatePickUp::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
    }
    ImGui::End();
}

CEREAL_REGISTER_TYPE(ComponentStatePickUp)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentStatePickUp)
