#pragma once
#include "ComponentPickUp.h"
#include <System/Component/ComponentModel.h>
#include <System/VectorMath.h>

void ComponentPickUp::ExecutePickUp()
{
    if(!throwable_obj_)
        return;
    auto throwable_owner = throwable_obj_->GetOwner();
    auto capsule         = throwable_owner->GetComponent<ComponentCollisionCapsule>();
    capsule->SetOverlapCollisionGroup((u32)(ComponentCollision::CollisionGroup::PLAYER));
    throwable_obj_->PickUp();
}

void ComponentPickUp::ExecuteThrow()
{
    if(!throwable_obj_)
        return;
    auto throwable_owner = throwable_obj_->GetOwner();
    auto capsule         = throwable_owner->GetComponent<ComponentCollisionCapsule>();
    // プレイヤーカプセルは見えないため、投げ物と物理的に押し合うと何も無いところで跳ねる。
    // 接触しても押し合わないようにPLAYERとはオーバーラップのままにする(当たり検出はプレイヤーの球側が担当)
    capsule->SetOverlapCollisionGroup((u32)(ComponentCollision::CollisionGroup::PLAYER));

    // プレイヤーモデルの向いている方向へ投げる
    float3 dir{0, 0, 0};
    if(auto mdl = GetOwner()->GetComponent<ComponentModel>()) {
        matrix m = mdl->GetWorldMatrix();
        dir      = {-m._31, m._32, -m._33};
        dir.y    = 0.0f;
        if((float)length(dir) > 0.001f)
            dir = normalize(dir);
    }
    if((float)length(dir) < 0.001f)
        dir = -GetOwner()->GetWorldMatrix().axisZ();

    // 離脱直後に自分のカプセル顶部へめり込まないよう、前方ぶんずらして解き放つ
    float3 side = cross(float3(0, 1.0f, 0.0f), dir);
    throwable_owner->SetTranslate(GetOwner()->GetWorldMatrix().translate() + float3(0, 30.0f, 0) + dir * 5.0f - side * (capsule->GetHeight() * 0.5f));

    throwable_obj_->Throw(dir);
}

void ComponentPickUp::Update()
{
    __super::Update();

    switch(state_) {
    case PickUpState::None:
        if(Input::IsKeyDown(KEY_INPUT_SPACE) && throwable_obj_ && can_pickUp_) {
            ExecutePickUp();
            state_ = PickUpState::Holding;
        }
        break;
    case PickUpState::Holding:
        if(Input::IsKeyDown(KEY_INPUT_SPACE)) {
            ExecuteThrow();
            state_ = PickUpState::None;
        }
        break;
    default:
        break;
    }
}

void ComponentPickUp::LateUpdate()
{
    __super::LateUpdate();

    // 持ち上げ中はプレイヤーの頭上を追従させる
    if(state_ == PickUpState::Holding) {
        if(throwable_obj_) {
            auto throwable_owner = throwable_obj_->GetOwner();

            // カプセルの長い軸(オーナーのY軸)を正面に対して左右横向きに向き合わせる
            float3 side{0, 0, 0};
            if(auto mdl = GetOwner()->GetComponent<ComponentModel>()) {
                matrix m = mdl->GetWorldMatrix();
                float3 forward = {-m._31, m._32, -m._33};
                side           = cross(float3(0, 1.0f, 0.0f), forward);
                if((float)length(forward) > 0.001f && (float)length(side) > 0.001f) {
                    forward = normalize(forward);
                    side    = normalize(side);
                    throwable_owner->SetVectorAxisX(forward);
                    throwable_owner->SetVectorAxisY(side);
                    throwable_owner->SetVectorAxisZ(cross(forward, side));
                } else {
                    side = {0, 0, 0};
                }
            }

            // カプセルの中心が頭上に来るよう、原点を高さの半分ぶん逆方向へ寄せる
            float3 offset{0, 30.0f, 0};
            if(auto capsule = throwable_owner->GetComponent<ComponentCollisionCapsule>())
                offset -= side * (capsule->GetHeight() * 0.5f);
            throwable_owner->SetTranslate(GetOwner()->GetWorldMatrix().translate() + offset);
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

            ImGui::Text("State: %d", (int)state_);

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
