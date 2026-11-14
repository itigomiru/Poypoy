//---------------------------------------------------------------------------
//!	@file	PoyPoy_Bomb.cpp
//! @brief	PoyPoy_Bomb
//---------------------------------------------------------------------------

#include "PoyPoy_Bomb.h"
#include "Component/ComponentThrowable.h"
#include "Component/ComponentCrouch.h"
#include "Component/ComponentKnockback.h"
#include "PoyPoy_Character.h"
#include <System/Component/ComponentCollisionSphere.h>
#include <System/Component/ComponentEffect.h>
#include <System/Scene.h>
#include <algorithm>

namespace PoyPoy {
//! @brief 初期化
//! @return 初期化終了
bool Bomb::Init()
{
    // コリジョン生成前に設計値を設定する(派生クラスが数値を変える)
    InitSettings();

    Super::Init();

    SetName(BombName());

    // 投げ物として実ヒット(オーバーラップ除く)したら爆発する
    if(auto throwable = GetComponent<ComponentThrowable>()) {
        std::weak_ptr<Bomb> weak = std::dynamic_pointer_cast<Bomb>(SharedThis());
        throwable->SetContactFunc([weak]() {
            if(auto self = weak.lock(); self && !self->exploded_) {
                auto th = self->GetComponent<ComponentThrowable>();
                // 投げて攻撃判定がONのものだけが着弾で爆発する(設置状態は爆発しない)
                if(th && th->IsArmed())
                    self->Explode();
            }
        });
    }

    return true;
}

//! @brief 更新(誘爆予約の残りフレームを進める。予約が尽きたら継承リストで爆発する)
void Bomb::Update()
{
    Super::Update();

    if(exploded_ || chain_frames_ < 0)
        return;

    if(--chain_frames_ > 0)
        return;

    // 予約時刻到来。元爆風の当たり済みリストを受け継いで爆発する
    Explode(pending_victims_);
}

//! @brief 誘爆の予約(爆風に当たったら chain_delay_frames_ ぶん待ってから爆発する。
//!        爆風は数フレーム当たり続けるので、予約済みなら再予約しない)
void Bomb::ChainDetonate(const std::shared_ptr<std::vector<void*>>& chain_victims)
{
    if(exploded_ || chain_frames_ >= 0)
        return;

    chain_frames_    = chain_delay_frames_;
    pending_victims_ = chain_victims;
}

//! @brief 設計値の設定(重くて跳ねにくい。爆発するのでバウンドは抑える)
void Bomb::InitSettings()
{
    SetModelPath("");    // ひとまずモデルなし(爆弾モデルが用意できたらパスを設定する)
    SetRadius(4.0f);
    SetHeight(12.0f);
    SetThrowSpeed(45.0f);
    SetWeight(30.0f);
    SetBounce(0.15f);
    SetFriction(20.0f);
    SetGravityAccel(98.0f);
    SetSpin(120.0f);
    SetDamage(30.0f);     // 爆風ダメージ(しゃがみ中は倍率軽減)
    SetLiftTime(3.0f);    // 投げてすぐ着弾爆発するので短く持つ(上限値は ComponentPickUp 側が実装するまで未反映)
}

//! @brief 設計値の設定(標準のボムから小さい分だけ上書き)
void SmallBomb::InitSettings()
{
    Super::InitSettings();

    SetRadius(2.5f);
    SetHeight(8.0f);
    SetThrowSpeed(55.0f);    // 軽いので標準より速めに飛ばす
    SetWeight(15.0f);
    SetDamage(15.0f);
    SetLiftTime(4.0f);
    SetBlastRadius(25.0f);
}

//! @brief 爆発処理(一瞬大きい球コリジョンの爆風を発生させて消える)
void Bomb::Explode(const std::shared_ptr<std::vector<void*>>& chain_victims)
{
    // 誘爆や当たり多重で複数回呼ばれても一回だけ爆発する
    if(exploded_)
        return;
    exploded_  = true;
    float3 pos = GetWorldMatrix().translate();

    // 爆風(一瞬だけ大きい球コリジョン。ダメージとノックバックはBlast側が処理/投げ主は除外/爆弾はノックバック2倍)
    // 誘爆の爆風は元爆風の当たり済みリストを共有するので、同じキャラにダメージが二重にかからない
    auto throwable = GetComponent<ComponentThrowable>();
    Blast::Spawn(pos, blast_radius_, GetDamage(), throwable ? throwable->GetThrower() : std::weak_ptr<Object>{}, GetKnockbackScale(), chain_victims);

    // 爆発エフェクト(再生終了で自動消滅)
    float s = blast_radius_ / 6.0f;
    ComponentEffect::Object::Create("data/Game/Models/Effect/hanabi.efkefc", pos, float3(0, 0, 0), float3(s, s, s));

    // 爆発した爆弾は消える
    Scene::Object::Release(SharedThis());
}

//! @brief 爆風を発生させる(指定位置に指定半径の球コリジョンを作り、数フレームで消える)
void Blast::Spawn(const float3& pos, float radius, float damage, const std::weak_ptr<Object>& thrower, float knockback_scale,
                  const std::shared_ptr<std::vector<void*>>& chain_victims)
{
    auto blast              = Scene::Object::Create<Blast>();
    blast->radius_          = radius;
    blast->damage_          = damage;
    blast->thrower_         = thrower;
    blast->knockback_scale_ = knockback_scale;
    // 誘爆の爆風は元爆風と同じ当たり済みリストを共有する(同一連鎖でダメージの二重掛け防止)
    blast->victims_         = chain_victims ? chain_victims : std::make_shared<std::vector<void*>>();

    // 半径はInitの時点では未知なのでここで設定する
    if(auto sphere = blast->GetComponent<ComponentCollisionSphere>())
        sphere->SetRadius(radius);

    blast->SetTranslate(pos);
}

//! @brief 初期化(大きい球コリジョンを生成する)
//! @return 初期化終了
bool Blast::Init()
{
    Super::Init();

    SetName("Blast");

    // キャラ体躯とボムにはオーバーラップ(物理的に押し合わず当たりコールバックだけ発火)で検出する。
    // ボムを検出することで爆風がボムに当たった誘爆ができる。
    // ノックバック受け(ENEMY/hits=THROWABLE)や拾得判定(PLAYER/hits=THROWABLE)は対象外なので影響しない
    AddComponent<ComponentCollisionSphere>()
        ->SetCollisionGroup(ComponentCollision::CollisionGroup::ETC)
        ->SetHitCollisionGroup((u32)ComponentCollision::CollisionGroup::PLAYER | (u32)ComponentCollision::CollisionGroup::ENEMY |
                                (u32)ComponentCollision::CollisionGroup::THROWABLE)
        ->SetOverlapCollisionGroup((u32)ComponentCollision::CollisionGroup::PLAYER | (u32)ComponentCollision::CollisionGroup::ENEMY |
                                    (u32)ComponentCollision::CollisionGroup::THROWABLE);

    return true;
}

//! @brief 更新(短時間後に消える)
void Blast::Update()
{
    Super::Update();

    if(--life_ <= 0)
        Scene::Object::Release(SharedThis());
}

//! @brief 描画(爆風球を一瞬表示する)
void Blast::Draw()
{
    DrawSphere3D(cast(GetTranslate()), radius_, 16, GetColor(255, 150, 0), GetColor(255, 150, 0), TRUE);
}

//! @brief 当たり処理(当たったCharacterへダメージとノックバックを適用、当たったBombは誘爆)
void Blast::OnHit(const ComponentCollision::HitInfo& hit_info)
{
    if(!hit_info.hit_ || !hit_info.hit_collision_)
        return;

    auto owner = hit_info.hit_collision_->GetOwnerPtr();

    // 爆風がボムに当たったら誘爆を予約する(即爆発ではなく少し待ってから爆発/同じ当たり済みリストを渡す/Explode側で一回だけ爆発)
    if(auto bomb = std::dynamic_pointer_cast<Bomb>(owner)) {
        bomb->ChainDetonate(victims_);
        return;
    }

    // キャラクターのみ対象(地面や壁はグループ設定で当たらない)
    auto character = std::dynamic_pointer_cast<Character>(owner);
    if(!character || !character->IsAlive() || character->IsDying())
        return;

    // 投げ主は爆風の影響を受けない
    if(auto thrower = thrower_.lock(); thrower && thrower == owner)
        return;

    // 球は数フレーム重なり続けるので、同じ爆風連鎖で対象ごとに一回だけ適用する(誘爆先ともリスト共有)
    void* id = character.get();
    if(std::find(victims_->begin(), victims_->end(), id) != victims_->end())
        return;
    victims_->push_back(id);

    // ダメージ(しゃがみ中は倍率軽減/HP0なら消滅)
    float damage = damage_;
    if(auto crouch = character->GetComponent<ComponentCrouch>(); crouch && crouch->IsCrouching())
        damage *= crouch->GetDamageScale();
    character->ApplyDamage(damage);

    // 吹き飛ばし(しゃがみ・無敵時間の判定はComponentKnockback側で行う/爆弾由来の爆風は倍率2倍)
    if(!character->IsAlive())
        return;
    if(auto knock = character->GetComponent<ComponentKnockback>())
        knock->KnockBack(character->GetWorldMatrix().translate() - GetWorldMatrix().translate(), knockback_scale_);
}

}    // namespace PoyPoy
