#pragma once
#include "ComponentThrowable.h"

void ComponentThrowable::Init()
{
    __super::Init();
}

void ComponentThrowable::Update()
{
    __super::Update();
    owner_->GetComponent<ComponentCollisionCapsule>()->SetCollisionGroup(ComponentCollision::CollisionGroup::THROWABLE);
}

void ComponentThrowable::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();

        if(ImGui::TreeNode("Throwable")) {
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

void ComponentThrowable::Release()
{
    owner_->RemoveComponent<ComponentAttachModel>();
    owner_->GetComponent<ComponentPhysics>()->SetPhysicsReal(1.0f);
}

CEREAL_REGISTER_TYPE(ComponentThrowable)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentThrowable)
