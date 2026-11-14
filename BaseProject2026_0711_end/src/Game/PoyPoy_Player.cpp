//---------------------------------------------------------------------------
//!	@file	PoyPoy_Player.cpp
//! @brief	PoyPoy_Player
//---------------------------------------------------------------------------

#include "PoyPoy_Player.h"
#include "InGame.h"
#include "Component/ComponentStateIdleWalk.h"
#include "Component/ComponentCameraController.h"
#include "Component/ComponentStatePickUp.h"
#include "Component/ComponentPlayerState.h"
#include "Component/ComponentThrowable.h"
#include "PoyPoy_ThrowableObject.h"
#include "Component/ComponentPickUp.h"
#include "Component/ComponentInput.h"

namespace PoyPoy {
//! @brief 初期化
//! @return 初期化終了
bool Player::Init()
{
    Super::Init();

    SetName("Player");

    //auto player = Scene::Object::Create<Object>("Player");
    AddComponent<ComponentModel>("data/Game/Models/Player/Player1.mv1")
        ->SetAnimation({
            { "idle", "data/Game/Models/Player/Anims/Idle.mv1", 1, 1.0f}, // Idle
            { "walk", "data/Game/Models/Player/Anims/Walk.mv1", 1, 1.0f}, // Walk
            {"throw", "data/Game/Models/Player/Anims/Lift.mv1", 1, 1.0f}, // Lift
    })
        ->SetScaleAxisXYZ({0.1f, 0.1f, 0.1f});

    SetTranslate({0, 0, 0});

    AddComponent<ComponentPlayerState>();

    AddComponent<ComponentCameraController>();

    auto col = AddComponent<ComponentCollisionCapsule>()    //
                   ->SetRadius(5.0f)
                   ->SetHeight(21.0f)
                   ->SetTranslate(float3(0, 0, 0));

    col->UseGravity();
    col->SetCollisionGroup(ComponentCollision::CollisionGroup::PLAYER);

    // キャラ同士(自機とAI機)は当たりをすり抜ける
    // すり抜けないとカプセル分までしか近づけず、AIが攻撃距離に入れなくなる
    col->SetOverlapCollisionGroup((u32)ComponentCollision::CollisionGroup::PLAYER);

    AddComponent<ComponentCollisionSphere>()
        ->SetRadius(5.0f)
        ->SetOverlapCollisionGroup((u32)ComponentCollision::CollisionGroup::THROWABLE)
        ->SetHitCollisionGroup((u32)ComponentCollision::CollisionGroup::THROWABLE)
        ->SetCollisionGroup(ComponentCollision::CollisionGroup::PLAYER);

    AddComponent<ComponentPickUp>();

    return true;
}

void Player::Update()
{
    Super::Update();
    // モデルコンポーネントを取得
    if(auto mdl = GetComponent<ComponentModel>()) {
        // モデルのワールドマトリクスを取得
        matrix m = mdl->GetWorldMatrix();

        // モデルの前方ベクトルを取り出し、前方かつ足元に球衝突判定を配置する
        float3 forward = {-m._31, m._32, -m._33};
        if((float)length(forward) > 0.0f) {
            forward = normalize(forward);
        }

        // 前方へ 5.0f、足元へはカプセルの translate を基準に下方向へ配置
        float3 offsetPos = forward * 7.0f + float3(0.0f, -3.0f, 0.0f);

        if(auto colSphere = GetComponent<ComponentCollisionSphere>()) {
            colSphere->SetTranslate(offsetPos);
        }
    }
}

//---------------------------------------------------------------------------
//! @name 操作タイプ
//---------------------------------------------------------------------------
//! @brief 操作タイプを設定します(入力ソースを切り替えます)
//! @param type 操作タイプ
//! @return 自分のSharedPtr
PlayerPtr Player::SetControlType(ControlType type)
{
    ComponentInput::Create(this, type);
    return std::dynamic_pointer_cast<Player>(shared_from_this());
}

//! @brief 現在の操作タイプを取得します
//! @return 操作タイプ
ControlType Player::GetControlType() const
{
    if(auto input = GetInput())
        return input->GetControlType();

    return ControlType::Player;
}

//! @brief 入力ソースを取得します
//! @return 入力ソース(未設定時は既定の物理入力)
ComponentInputPtr Player::GetInput() const
{
    return GetComponent<ComponentInput>();
}

void Player::OnHit(const ComponentCollision::HitInfo& hitInfo)
{
    Super::OnHit(hitInfo);

    auto group = hitInfo.hit_collision_->GetCollisionGroup();

    if(group == ComponentCollision::CollisionGroup::THROWABLE) {
        if(auto pickUp = GetComponent<ComponentPickUp>()) {
            pickUp->SetCanPickUp(true);
            pickUp->SetThrowableObj(hitInfo.hit_collision_->GetOwner()->GetComponent<ComponentThrowable>());
        }
    }
    // キャラ同士は当たりをすり抜ける設定にしても当たり自体は毎フレーム届く。
    // すり抜けた当たりで拾得状態を消すと、is_pickup_ のまま手持ちの投げ物が外せなくなる
    else if(group != ComponentCollision::CollisionGroup::PLAYER) {
        if(auto pickUp = GetComponent<ComponentPickUp>()) {
            pickUp->SetCanPickUp(false);
            pickUp->SetThrowableObj(nullptr);
        }
    }
}

}    // namespace PoyPoy
