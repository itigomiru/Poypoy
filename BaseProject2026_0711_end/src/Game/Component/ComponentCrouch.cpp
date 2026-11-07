//---------------------------------------------------------------------------
//! @file   ComponentCrouch.cpp
//! @brief  しゃがみコンポーネント
//---------------------------------------------------------------------------
#pragma once
#include <Game/Component/ComponentCrouch.h>
#include <Game/Component/ComponentKnockback.h>
#include <System/Input/InputKey.h>

void ComponentCrouch::Init()
{
    __super::Init();

    SetName<Component>("Crouch");

    // 縮小前のモデルスケールを覚えておく(しゃがみ解除時の復帰用)
    if(auto mdl = GetOwner()->GetComponent<ComponentModel>()) {
        mdl_        = mdl;
        base_scale_ = mdl->GetScaleAxisXYZ();
    }
}

void ComponentCrouch::Update()
{
    __super::Update();

    if(!GetStatus(StatusBit::Enable)) {
        SetCrouching(false);
        return;
    }

    // ノックバック中・無敵時間中はしゃがみ入力を無効にする(ノックバックの移動に任せる)
    if(auto knock = GetOwner()->GetComponent<ComponentKnockback>()) {
        if(knock->IsKnockingBack() || knock->IsInvincible()) {
            SetCrouching(false);
            return;
        }
    }

    // キー長押し中にしゃがむ
    SetCrouching(IsKey(key_crouch_));
}

void ComponentCrouch::SetCrouching(bool crouching)
{
    if(crouching_ == crouching)
        return;

    crouching_ = crouching;

    // しゃがみ中はモデルを縮小してしゃがみを視覚に出す
    if(auto mdl = mdl_.lock()) {
        float3 scale = base_scale_;
        if(crouching_)
            scale *= crouch_scale_;
        mdl->SetScaleAxisXYZ(scale);
    }
}

void ComponentCrouch::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode("Crouch")) {
            // 有効/無効
            bool enable = GetStatus(StatusBit::Enable);
            if(ImGui::Checkbox(u8"有効", &enable))
                SetStatus(StatusBit::Enable, enable);

            ImGui::Text(u8"しゃがみ中: %s", crouching_ ? u8"はい" : u8"いいえ");
            ImGui::SliderFloat(u8"移動倍率", &move_scale_, 0.0f, 1.0f);
            ImGui::SliderFloat(u8"ダメージ倍率", &damage_scale_, 0.0f, 1.0f);
            ImGui::SliderFloat(u8"モデル縮小率", &crouch_scale_, 0.1f, 1.0f);

            // GUI上でオーナーから自分を削除します
            if(ImGui::Button(u8"削除"))
                GetOwner()->RemoveComponent(shared_from_this());

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

CEREAL_REGISTER_TYPE(ComponentCrouch)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentCrouch)
