#pragma once
#include "ComponentPickUp.h"
#include <Game/PoyPoy_Throwable.h>
#include <Game/PoyPoy_Character.h>
#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentCollisionCapsule.h>
#include <System/Component/ComponentCollisionSphere.h>
#include <System/Component/ComponentCollisionBox.h>
#include <System/VectorMath.h>

namespace {
//! @brief 投げ物コリジョンをPLAYERとオーバーラップ設定にする(所持中は押し合い跳过し。カプセル・ボックス両対応)
void SetThrowableOverlapPlayer(Object* owner)
{
    const u32 player = (u32)ComponentCollision::CollisionGroup::PLAYER;
    if(auto capsule = owner->GetComponent<ComponentCollisionCapsule>())
        capsule->SetOverlapCollisionGroup(player);
    else if(auto sphere = owner->GetComponent<ComponentCollisionSphere>())
        sphere->SetOverlapCollisionGroup(player);
    else if(auto box = owner->GetComponent<ComponentCollisionBox>())
        box->SetOverlapCollisionGroup(player);
}

//! @brief 投げ物を頭上で横向きにずらすぶんのコリジョン半幅(カプセルは高さ半分/スフィアは中心が原点なので真上のままでよい/ボックスはX方向サイズ)
float ThrowableHalfSpread(Object* owner)
{
    if(auto capsule = owner->GetComponent<ComponentCollisionCapsule>())
        return capsule->GetHeight() * 0.5f;
    if(auto sphere = owner->GetComponent<ComponentCollisionSphere>())
        return 0.0f;
    if(auto box = owner->GetComponent<ComponentCollisionBox>())
        return box->GetSize().x;
    return 0.0f;
}
}    // namespace

void ComponentPickUp::ExecutePickUp()
{
    if(!throwable_obj_)
        return;
    SetThrowableOverlapPlayer(throwable_obj_->GetOwner());

    // 保持時間上限を初期化する(投げ物側の lift_time。-1は無制限。
    // パワーが高いほど長く持てるので、パワーアップアイテムは重い物の短い保持時間の不利を和らげる)
    float limit = -1.0f;
    if(auto throwable = std::dynamic_pointer_cast<PoyPoy::Throwable>(throwable_obj_->GetOwnerPtr()))
        limit = throwable->GetLiftTime();
    if(limit >= 0.0f) {
        if(auto character = std::dynamic_pointer_cast<PoyPoy::Character>(GetOwnerPtr()))
            limit *= character->GetPower() / 10.0f;    // 基準パワー10で倍率化する
    }
    hold_time_ = limit;

    throwable_obj_->PickUp();
}

void ComponentPickUp::ExecuteThrow()
{
    if(!throwable_obj_)
        return;
    auto throwable_owner = throwable_obj_->GetOwner();
    // プレイヤーカプセルは見えないため、投げ物と物理的に押し合うと何も無いところで跳ねる。
    // 接触しても押し合わないようにPLAYERとはオーバーラップのままにする(当たり検出はプレイヤーの球側が担当)
    SetThrowableOverlapPlayer(throwable_owner);

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
    throwable_owner->SetTranslate(GetOwner()->GetWorldMatrix().translate() + float3(0, 30.0f, 0) + dir * 5.0f - side * ThrowableHalfSpread(throwable_owner));

    // 誰が投げたか覚えておく(爆弾など着弾効果で投げ主を攻撃しないための識別用)
    throwable_obj_->SetThrower(GetOwnerPtr());
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
        // 保持時間上限を反映する(重い物は lift_time が短く、時間が来たら自動で投げる。
        // パワーアップ中(パワー大)なら上限が伸びて長く持てる)
        if(hold_time_ >= 0.0f) {
            hold_time_ -= update_delta_time_ > 0.0f ? update_delta_time_ : 1.0f / 60.0f;
            if(hold_time_ <= 0.0f) {
                ExecuteThrow();
                state_ = PickUpState::None;
                break;
            }
        }
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
                matrix m       = mdl->GetWorldMatrix();
                float3 forward = {-m._31, m._32, -m._33};
                side           = cross(float3(0, 1.0f, 0.0f), forward);
                if((float)length(forward) > 0.001f && (float)length(side) > 0.001f) {
                    forward = normalize(forward);
                    side    = normalize(side);
                    throwable_owner->SetVectorAxisX(forward);
                    throwable_owner->SetVectorAxisY(side);
                    throwable_owner->SetVectorAxisZ(cross(forward, side));
                }
                else {
                    side = {0, 0, 0};
                }
            }

            // コリジョンの中心が頭上に来るよう、原点を形状の半幅ぶん逆方向へ寄せる(カプセル・ボックス両対応)
            float3 offset{0, 30.0f, 0};
            offset -= side * ThrowableHalfSpread(throwable_owner);
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
            if(hold_time_ >= 0.0f)
                ImGui::Text("HoldLeft: %.1f", hold_time_);

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
