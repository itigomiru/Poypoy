//---------------------------------------------------------------------------
//! @file   ComponentDodge.cpp
//! @brief  前転回避コンポーネント
//---------------------------------------------------------------------------
#pragma once
#include <Game/Component/ComponentDodge.h>
#include <Game/Component/ComponentKnockback.h>
#include <Game/Component/ComponentCrouch.h>
#include <System/Input/InputKey.h>

void ComponentDodge::Init()
{
    __super::Init();

    SetName<Component>("Dodge");

    // 前転開始時の正面取得用のモデルを覚えておく
    if(auto mdl = GetOwner()->GetComponent<ComponentModel>())
        mdl_ = mdl;
}

void ComponentDodge::Update()
{
    __super::Update();

    auto owner = GetOwner();

    if(!GetStatus(StatusBit::Enable)) {
        dodging_      = false;
        dodge_queued_ = false;
        return;
    }

    // ノックバック状態は前転の開始ブロックと前転中の中止の両方に使う
    auto knock = owner->GetComponent<ComponentKnockback>();

    if(dodging_) {
        // 前転中にノックバックを食らったら前転と予約を中止してノックバックに移動を譲る
        // (自分で付与した無敵時間では中止しない)
        if(knock && knock->IsKnockingBack()) {
            dodging_      = false;
            dodge_queued_ = false;
            return;
        }

        // しゃがみが入ったら前転と予約を中止してしゃがみを優先する
        if(auto crouch = owner->GetComponent<ComponentCrouch>()) {
            if(crouch->IsCrouching()) {
                dodging_      = false;
                dodge_queued_ = false;
                return;
            }
        }

        // 前転中に再度押したら次の前転を予約する(前転終了直後に開始される)
        if(Input::IsKeyDown(key_dodge_))
            dodge_queued_ = true;

        roll_left_ -= GetDeltaTime();
        if(roll_left_ <= 0.0f) {
            dodging_ = false;

            // 予約があればそのまま次の前転へ連続開始する
            if(dodge_queued_) {
                dodge_queued_ = false;
                TryStartDodge();
            }
            return;
        }

        // 前転開始時に捕捉した正面方向へ移動する
        owner->AddTranslate(roll_dir_ * roll_speed_, true);

        // 前転中は前進の見た目に歩いてもらう
        if(auto mdl = mdl_.lock())
            mdl->PlayAnimationNoSame("walk", true);
        return;
    }

    // 回避キーの押下で前転開始
    if(Input::IsKeyDown(key_dodge_))
        TryStartDodge();
}

void ComponentDodge::TryStartDodge()
{
    // ノックバック中・ノックバック後の無敵時間中は開始できない
    if(auto knock = GetOwner()->GetComponent<ComponentKnockback>()) {
        if(knock->IsKnockInvincible())
            return;
    }

    // しゃがみ中は開始できない
    if(auto crouch = GetOwner()->GetComponent<ComponentCrouch>()) {
        if(crouch->IsCrouching())
            return;
    }

    StartDodge();
}

void ComponentDodge::StartDodge()
{
    // モデルの向いている水平方向を前転方向に捕捉する
    roll_dir_ = {0.0f, 0.0f, -1.0f};
    if(auto mdl = mdl_.lock()) {
        roll_dir_ = -mdl->GetWorldVectorAxisZ();
        roll_dir_.y = 0.0f;
    }

    if((float)length(roll_dir_) <= 0.0f)
        roll_dir_ = {0.0f, 0.0f, -1.0f};
    else
        roll_dir_ = normalize(roll_dir_);

    dodging_   = true;
    roll_left_ = roll_time_;

    // 押してから無敵時間を与える(点滅なし。無敵時間が切れた後に当たれば通常通りノックバックする)
    if(auto knock = GetOwner()->GetComponent<ComponentKnockback>())
        knock->StartSilentInvincible(invincible_time_);
}

void ComponentDodge::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode("Dodge")) {
            // 有効/無効
            bool enable = GetStatus(StatusBit::Enable);
            if(ImGui::Checkbox(u8"有効", &enable))
                SetStatus(StatusBit::Enable, enable);

            ImGui::Text(u8"前転中: %s", dodging_ ? u8"はい" : u8"いいえ");
            ImGui::SliderFloat(u8"前転速度", &roll_speed_, 0.0f, 2.0f);
            ImGui::SliderFloat(u8"前転時間", &roll_time_, 0.05f, 1.0f);
            ImGui::SliderFloat(u8"無敵時間", &invincible_time_, 0.0f, 1.0f);

            // GUI上でオーナーから自分を削除します
            if(ImGui::Button(u8"削除"))
                GetOwner()->RemoveComponent(shared_from_this());

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

CEREAL_REGISTER_TYPE(ComponentDodge)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentDodge)
