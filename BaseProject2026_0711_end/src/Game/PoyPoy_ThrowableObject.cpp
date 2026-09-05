//---------------------------------------------------------------------------
//!	@file	PoyPoy_ThrowableObject.cpp
//! @brief	PoyPoy_ThrowableObject
//---------------------------------------------------------------------------

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

    AddComponent<ComponentModel>("data/Sample/Coin/GoldCoin.mv1");

    AddComponent<ComponentCollisionSphere>()    //
        ->SetRadius(3.0f)
        ->SetMass(10.0f)
        ->SetRotationAxisXYZ({90.0f, 0.0f, 0.0f})
        ->SetTranslate({0.0f, 0.0f, 0.0f})
        ->SetCollisionGroup(ComponentCollision::CollisionGroup::THROWABLE)
        ->SetHitCollisionGroup((u32)ComponentCollision::CollisionGroup::GROUND | (u32)ComponentCollision::CollisionGroup::WALL |
                               (u32)ComponentCollision::CollisionGroup::PLAYER)
        ->UseGravity();

    return true;
}

void ThrowableObject::Update()
{
    Super::Update();
}

// 当たり判定が行われたときに呼ばれる関数
void ThrowableObject::OnHit(const ComponentCollision::HitInfo& hit_info)
{
    Super::OnHit(hit_info);
}

}    // namespace PoyPoy
