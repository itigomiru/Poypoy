//---------------------------------------------------------------------------
//! @file   ComponentKnockback.cpp
//! @brief  ノックバックコンポーネント
//---------------------------------------------------------------------------
#pragma once
#include <Game/Component/ComponentKnockback.h>
#include <Game/Component/ComponentThrowable.h>
#include <Game/Component/ComponentCrouch.h>
#include <Game/PoyPoy_Character.h>
#include <Game/PoyPoy_Throwable.h>
#include <cmath>

void ComponentKnockback::Init()
{
    __super::Init();

    SetName<Component>("Knockback");

    // 体躯と同じ形状の受け用コリジョンを自分で追加する。THROWABLEにだけ物理で当たる
    auto target = GetOwner()->AddComponent<ComponentCollisionCapsule>()    //
                      ->SetRadius(target_radius_)
                      ->SetHeight(target_height_)
                      ->SetMass(target_mass_)
                      ->SetTranslate(target_translate_)
                      ->SetCollisionGroup(ComponentCollision::CollisionGroup::ENEMY)
                      ->SetHitCollisionGroup((u32)ComponentCollision::CollisionGroup::THROWABLE);

    std::weak_ptr<ComponentKnockback> weak = std::dynamic_pointer_cast<ComponentKnockback>(shared_from_this());
    target->OnHitFunc = [weak](const ComponentCollision::HitInfo& hit_info) {
        if(auto self = weak.lock())
            self->OnKnockHit(hit_info);
    };

    target_ = target;

    if(auto mdl = GetOwner()->GetComponent<ComponentModel>())
        mdl_ = mdl;
}

void ComponentKnockback::Update()
{
    __super::Update();

    float dt = GetDeltaTime();

    if(knocking_) {
        // ノックバック中は移動して、摩擦で減速して止まる
        float damp = 1.0f - knock_friction_ * dt;
        knock_velocity_ *= damp > 0.0f ? damp : 0.0f;

        if((float)dot(knock_velocity_, knock_velocity_) >= 0.01f) {
            GetOwner()->SetTranslate(GetOwner()->GetWorldMatrix().translate() + knock_velocity_ * dt);
            return;
        }

        // ノックバック終了: ここから無敵時間をカウントする(当たり判定なしは継続)
        knock_velocity_  = {0.0f, 0.0f, 0.0f};
        knocking_        = false;
        invincible_left_ = invincible_time_;
        flash_on_        = true;
        flash_left_      = flash_interval_;
        return;
    }

    if(invincible_left_ > 0.0f) {
        invincible_left_ -= dt;
    if(dodge_invincible_left_ > 0.0f) {
        // 前転回避の無敵時間: 点滅させずに当たり判定なしだけ続ける
        dodge_invincible_left_ -= dt;
        if(dodge_invincible_left_ > 0.0f)
            return;

        // 前転回避の無敵時間終了: 当たり判定だけ戻す(モデルの表示は触らない)
        dodge_invincible_left_ = 0.0f;
        if(auto col = target_.lock())
            col->SetCollisionStatus(ComponentCollision::CollisionBit::DisableHit, false);
    }

    if(invincible_left_ > 0.0f) {
            // 無敵時間中: モデルを点滅させる
            flash_left_ -= dt;
            if(flash_left_ <= 0.0f) {
                flash_left_ = flash_interval_;
                flash_on_   = !flash_on_;
                if(auto mdl = mdl_.lock())
                    mdl->SetDrawMeshIndex(flash_on_ ? std::vector<int>{-1} : std::vector<int>{});
            }
            return;
        }

        // 無敵時間終了: 当たり判定とモデルの表示を戻す
        invincible_left_ = 0.0f;
        if(auto mdl = mdl_.lock())
            mdl->SetDrawMeshIndex(std::vector<int>{-1});
        if(auto col = target_.lock())
            col->SetCollisionStatus(ComponentCollision::CollisionBit::DisableHit, false);
    }
}

void ComponentKnockback::OnKnockHit(const ComponentCollision::HitInfo& hitInfo)
{
    if(!hitInfo.hit_ || !hitInfo.hit_collision_)
        return;

    // オーバーラップのペアは押し戻しが0なので実ヒットを絞る
    if((float)length(hitInfo.push_) <= 0.01f)
        return;

    if(hitInfo.hit_collision_->GetCollisionGroup() != ComponentCollision::CollisionGroup::THROWABLE)
        return;

    // ノックバック中・無敵時間中(前転回避の無敵時間も含む)は当たらない
    if(knocking_ || invincible_left_ > 0.0f || dodge_invincible_left_ > 0.0f)
        return;

    // 攻撃判定は投げたときだけONで当たって消えるので、無効な投げ物はノックバックしない
    ComponentThrowablePtr throwable = hitInfo.hit_collision_->GetOwner()->GetComponent<ComponentThrowable>();
    if(!throwable || !throwable->IsArmed())
        return;

    // 投げ物の速度の水平方向へノックバックする
    float3 vel = throwable->GetVelocity();
    vel.y = 0.0f;

    float speed = (float)sqrtf(dot(vel, vel));
    if(speed <= 0.01f)
        return;

    // しゃがみ中はノックバックを無効化し、ダメージを軽減する(しゃがみパラメータは爆風処理でも使う)
    auto crouch = GetOwner()->GetComponent<ComponentCrouch>();
    const bool crouching = crouch && crouch->IsCrouching();

    // 衝突で投げ物の前進の勢いを殺してその場に落とす(オーナーのノックバックと一緒に飛ぶのを防ぐ)
    throwable->SetVelocity({0.0f, throwable->GetVelocity().y, 0.0f});

    // オーナーがキャラクターなら投げ物側のダメージ分HPを減らす(Character側で0に丸める/しゃがみ中は倍率を掛ける)
    if(auto character = std::dynamic_pointer_cast<PoyPoy::Character>(GetOwnerPtr())) {
        if(auto throwable_type = dynamic_cast<PoyPoy::Throwable*>(hitInfo.hit_collision_->GetOwner())) {
            float damage = throwable_type->GetDamage();
            if(crouching)
                damage *= crouch->GetDamageScale();
            character->SetHP(character->GetHP() - damage);
        }
    }

    // しゃがみ中はノックバックしない(軽減したダメージのみ適用)
    if(crouching)
        return;

    // 投げ物の速度によらず常に同じ強さ(knock_speed_)でノックバックする
    knock_velocity_ = vel * (knock_speed_ / speed);

    // ノックバック中は当たり判定なしにして、もう一度当たったり押されたりしないようにする
    knocking_ = true;
    if(auto col = target_.lock())
        col->SetCollisionStatus(ComponentCollision::CollisionBit::DisableHit, true);
}

CEREAL_REGISTER_TYPE(ComponentKnockback)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentKnockback)
