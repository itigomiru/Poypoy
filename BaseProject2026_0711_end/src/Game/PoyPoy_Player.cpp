//---------------------------------------------------------------------------
//!	@file	PoyPoy_Player.cpp
//! @brief	PoyPoy_Player
//---------------------------------------------------------------------------

#include "PoyPoy_Player.h"
#include "PoyPoy_Bullet.h"
#include "InGame.h"
#include "Component/ComponentStateIdleWalk.h"
#include "Component/ComponentCameraController.h"
#include "Component/ComponentStatePickUp.h"
#include "Component/ComponentPlayerState.h"

namespace PoyPoy {
//! @brief 初期化
//! @return 初期化終了
bool Player::Init()
{
    Super::Init();

    SetName("Player");

    //auto player = Scene::Object::Create<Object>("Player");
    AddComponent<ComponentModel>("data/Game/Models/Player/player.mv1")
        ->SetAnimation({
            { "idle",  "data/Game/Models/Player/Anims/Idle.mv1", 1, 1.0f}, // Idle
            { "walk",  "data/Game/Models/Player/Anims/Walk.mv1", 1, 1.0f}, // Walk
            {"throw", "data/Game/Models/Player/Anims/Throw.mv1", 1, 1.0f}, // Throw
    })
        ->SetScaleAxisXYZ({0.001f, 0.001f, 0.001f});

    SetTranslate({0, 0, 0});

    AddComponent<ComponentPlayerState>();

    AddComponent<ComponentCameraController>();

    auto col = AddComponent<ComponentCollisionCapsule>()    //
                   ->SetRadius(5.0f)
                   ->SetHeight(21.0f)
                   ->SetTranslate(float3(0, -9.5f, 0));

    col->UseGravity();
    col->SetCollisionGroup(ComponentCollision::CollisionGroup::PLAYER);

    AddComponent<ComponentCollisionSphere>()
        ->SetTranslate(float3(0, 0, 10))
        ->SetRadius(5.0f)
        ->SetHitCollisionGroup((u32)ComponentCollision::CollisionGroup::THROWABLE);

    return true;
}

void Player::Update()
{
    Super::Update();
    // モデルコンポーネントを取得
    if(auto mdl = GetComponent<ComponentModel>()) {
        // モデルのワールドマトリクスを取得
        matrix m = mdl->GetWorldMatrix();

        // マトリクスから前方ベクトルを取り出す
        float3 forward = {-m._31, m._32, -m._33}; 
        if((float)length(forward) > 0.0f) {
            forward = normalize(forward);
        }

        // 前方へ10進んだ位置にスフィアを配置
        float3 offsetPos = forward * 5.0f;

        if(auto colSphere = GetComponent<ComponentCollisionSphere>()) {
            colSphere->SetTranslate(offsetPos);
        }
    }
}

}    // namespace PoyPoy
