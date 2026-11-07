#pragma once
#include <Game/Component/ComponentPlayerState.h>
#include <Game/Component/ComponentStatePickUp.h>
#include "ComponentStateIdleWalk.h"
#include "ComponentPickUp.h"
#include <Game/PoyPoy_Character.h>

void ComponentPlayerState::Init()
{
    __super::Init();

    auto idle = GetOwner()->AddComponent<ComponentStateIdleWalk>();

    // キャラクターならステータスの素早さを移動速度に使う
    if(auto character = std::dynamic_pointer_cast<PoyPoy::Character>(GetOwnerPtr())) {
        idle->SetMoveSpeed(character->GetMoveSpeed());
    }
    else {
        idle->SetMoveSpeed(0.3f);
    }
    idle->SetRotateSpeed(20.0f);
}

void ComponentPlayerState::Update()
{
    __super::Update();

    //! 今の状態を把握して状態を変えたい

    if(Input::IsKeyDown(KEY_INPUT_SPACE)) {
        //if(auto pickUp = owner->GetComponent<ComponentPickUp>()) {
        //    if(pickUp->GetCanPickUp() == true)
        //        ChangeState<ComponentStatePickUp>();
        //}
    }
}

void ComponentPlayerState::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode("Player State")) {
            // 有効/無効
            bool enable = GetStatus(StatusBit::Enable);
            if(ImGui::Checkbox(u8"有効", &enable))
                SetStatus(StatusBit::Enable, enable);

            // GUI上でオーナーから自分を削除します
            if(ImGui::Button(u8"削除"))
                GetOwner()->RemoveComponent(shared_from_this());

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

CEREAL_REGISTER_TYPE(ComponentPlayerState)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentPlayerState)
