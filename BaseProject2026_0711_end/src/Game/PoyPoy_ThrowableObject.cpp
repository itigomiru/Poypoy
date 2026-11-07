//---------------------------------------------------------------------------
//!	@file	PoyPoy_ThrowableObject.cpp
//! @brief	PoyPoy_ThrowableObject
//---------------------------------------------------------------------------
#include "Component/ComponentThrowable.h"
#include "PoyPoy_ThrowableObject.h"
#include "InGame.h"

namespace PoyPoy {
//int randomRange(int start, int end)
//{
//	return start + rand() % ((end - start) + 1);
//}

//! @brief 初期化
//! @return 初期化終了
bool ThrowableObject::Init()
{
    Super::Init();

    SetName("ThrowableObject");

    // ベースクラス(Throwable)の設定値から見た目とコリジョンを作る
    AddComponent<ComponentModel>(GetModelPath())->SetScaleAxisXYZ(GetModelScale());

    AddComponent<ComponentCollisionCapsule>()
        ->SetRadius(GetRadius())
        ->SetHeight(GetHeight())
        ->SetMass(10.0f)
        ->SetRotationAxisXYZ({0.0f, 0.0f, 0.0f})
        ->SetTranslate({0.0f, 0.0f, 0.0f})
        ->SetCollisionGroup(ComponentCollision::CollisionGroup::THROWABLE)
        ->SetHitCollisionGroup((u32)ComponentCollision::CollisionGroup::GROUND | (u32)ComponentCollision::CollisionGroup::WALL |
                               (u32)ComponentCollision::CollisionGroup::PLAYER |
                               (u32)ComponentCollision::CollisionGroup::ENEMY |
                               (u32)ComponentCollision::CollisionGroup::THROWABLE);

    SetRotationAxisXYZ({90.0f, 0.0f, 0.0f});
    // 重力・投げる挙動(初速度・重さ・バウンド・摩擦)は ComponentThrowable がベースの設定値を受けて扱う
    AddComponent<ComponentThrowable>();
    return true;
}

void ThrowableObject::Update()
{
    Super::Update();
}

}    // namespace PoyPoy
