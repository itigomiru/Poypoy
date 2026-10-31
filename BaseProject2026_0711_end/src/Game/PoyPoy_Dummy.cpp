//---------------------------------------------------------------------------
//!	@file	PoyPoy_Dummy.cpp
//! @brief	PoyPoy_Dummy
//---------------------------------------------------------------------------

#include "PoyPoy_Dummy.h"
#include "InGame.h"
#include <System/Component/ComponentModel.h>

namespace PoyPoy {
//! @brief 初期化
//! @return 初期化終了
bool Dummy::Init()
{
    Super::Init();

    SetName("Dummy");

    AddComponent<ComponentModel>("data/Game/Models/Player/Player1.mv1")
        ->SetAnimation({
            {"idle", "data/Game/Models/Player/Anims/Idle.mv1", 1, 1.0f},    // Idle
            {"walk", "data/Game/Models/Player/Anims/Walk.mv1", 1, 1.0f},    // Walk
    })
        ->SetScaleAxisXYZ({0.1f, 0.1f, 0.1f});

    // プレイヤーの横に立てる
    SetTranslate({12.0f, 5.0f, 0});

    auto col = AddComponent<ComponentCollisionCapsule>()    //
                   ->SetRadius(5.0f)
                   ->SetHeight(21.0f)
                   ->SetTranslate(float3(0, 0, 0));

    col->UseGravity();
    // ダミーは将来のプレイヤーそのものの扱い。投げ物はPLAYERとオーバーラップしてすり抜ける
    col->SetCollisionGroup(ComponentCollision::CollisionGroup::PLAYER);

    // 投げ物を当てるための体躯と同じ形状の受け用コリジョン。THROWABLEにだけ物理で当たる
    AddComponent<ComponentCollisionCapsule>()    //
        ->SetRadius(5.0f)
        ->SetHeight(21.0f)
        ->SetMass(50.0f)
        ->SetTranslate(float3(0, 0, 0))
        ->SetCollisionGroup(ComponentCollision::CollisionGroup::ENEMY)
        ->SetHitCollisionGroup((u32)ComponentCollision::CollisionGroup::THROWABLE);

    // 立ちアニメーションを再生する
    if(auto mdl = GetComponent<ComponentModel>()) {
        mdl->PlayAnimationNoSame("idle", true);
    }

    return true;
}

void Dummy::Update()
{
    Super::Update();
}

void Dummy::OnHit(const ComponentCollision::HitInfo& hitInfo)
{
    // 移動機能はないが、プレイヤーとは物理的にぶつかる
    Super::OnHit(hitInfo);
}

}    // namespace PoyPoy
