#pragma once
#include "ComponentPickUp.h"

void ComponentPickUp::Update()
{
    __super::Update();
    if(Input::IsKeyDown(KEY_INPUT_SPACE)) {
        if(throwable_obj_ && !is_pickup_) {
            SetThrowableObj(throwable_obj_);
            auto throwable_owner = throwable_obj_->GetOwner();
            throwable_owner->AddComponent<ComponentAttachModel>();
            throwable_owner->GetComponent<ComponentAttachModel>()->SetAttachObject(owner_, "mixamorig:RightHand", float3(1000.0f, 1000.0f, 1000.0f));
            throwable_owner->GetComponent<ComponentCollisionCapsule>()->SetOverlapCollisionGroup((u32)(ComponentCollision::CollisionGroup::PLAYER));
            throwable_owner->GetComponent<ComponentPhysics>()->SetPhysicsReal(0.0f);
            is_pickup_ = true;
            return;
        }
        if(throwable_obj_ && is_pickup_) {
            auto throwable_owner = throwable_obj_->GetOwner();
            throwable_owner->GetComponent<ComponentThrowable>()->Release();
            is_pickup_ = false;
        }
    }
}

void ComponentPickUp::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode("Pick Up")) {
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

CEREAL_REGISTER_TYPE(ComponentPickUp)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentPickUp)
