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

    AddComponent<ComponentModel>("data/Game/Models/Object/Crystal.mv1")->SetScaleAxisXYZ(0.2f);

    AddComponent<ComponentCollisionCapsule>()
        ->SetRadius(5.0f)
        ->SetHeight(38.0f)
        ->SetMass(10.0f)
        ->SetRotationAxisXYZ({0.0f, 0.0f, 0.0f})
        ->SetTranslate({0.0f, 0.0f, 0.0f})
        ->SetCollisionGroup(ComponentCollision::CollisionGroup::THROWABLE)
        ->SetHitCollisionGroup((u32)ComponentCollision::CollisionGroup::GROUND | (u32)ComponentCollision::CollisionGroup::WALL |
                               (u32)ComponentCollision::CollisionGroup::PLAYER)
        ->UseGravity();

    auto physics = AddComponent<ComponentPhysics>();
    physics->CreateCapsule(15.0f, 5.0f, 10.0f);
    physics->SetPhysicsReal(0.0f);
    physics->SetFriction(1.0f);
    physics->SetTranslate({0.0f, 18.0f, 0.0f});
    physics->SetGravityFactor(0.0f);

    SetRotationAxisXYZ({90.0f, 0.0f, 0.0f});
    AddComponent<ComponentThrowable>();
    return true;
}

void ThrowableObject::Update()
{
    Super::Update();
}

}    // namespace PoyPoy
