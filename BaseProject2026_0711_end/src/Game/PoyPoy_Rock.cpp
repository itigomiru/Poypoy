//---------------------------------------------------------------------------
//!	@file	PoyPoy_Rock.cpp
//! @brief	PoyPoy_Rock
//---------------------------------------------------------------------------

#include "PoyPoy_Rock.h"
#include "Component/ComponentThrowable.h"
#include <System/Component/ComponentCollisionSphere.h>
#include <System/Component/ComponentModel.h>

namespace PoyPoy {
//! @brief 初期化
//! @return 初期化終了
bool Rock::Init()
{
    // 重くて転がらない岩石の投げ物。見た目も重たげにゆっくり飛ぶ
    // 見た目は専用モデル未入手のためCube.xの暫定版
    SetModelPath("data/System/Cube.x");
    SetModelScale(radius_);
    SetThrowSpeed(40.0f);
    SetWeight(40.0f);
    SetBounce(0.05f);
    SetFriction(22.0f);
    SetGravityAccel(98.0f);
    SetSpin(0.0f);        // 転がさない
    SetDamage(15.0f);
    SetLiftTime(2.0f);    // 重いぶん短く持つ(上限が来ると自動で投げる。パワーで伸びる)

    Super::Init();

    SetName("Rock");

    if(!GetModelPath().empty())
        AddComponent<ComponentModel>(GetModelPath())->SetScaleAxisXYZ(GetModelScale());

    // スフィアコリジョン(ほかの投げ物と違うコリジョンタイプ)
    AddComponent<ComponentCollisionSphere>()
        ->SetRadius(radius_)
        ->SetMass(GetWeight())
        ->SetCollisionGroup(ComponentCollision::CollisionGroup::THROWABLE)
        ->SetHitCollisionGroup((u32)ComponentCollision::CollisionGroup::GROUND | (u32)ComponentCollision::CollisionGroup::WALL |
                               (u32)ComponentCollision::CollisionGroup::PLAYER |
                               (u32)ComponentCollision::CollisionGroup::ENEMY |
                               (u32)ComponentCollision::CollisionGroup::THROWABLE |
                               // ETC(爆風)にも当たりを設定し、ほかの投げ物と同じ扱いにしておく
                               (u32)ComponentCollision::CollisionGroup::ETC);

    // 重力・投げる挙動(初速度・重さ・バウンド・摩擦)は ComponentThrowable がベースの設定値を受けて扱う
    AddComponent<ComponentThrowable>();

    return true;
}

}    // namespace PoyPoy
