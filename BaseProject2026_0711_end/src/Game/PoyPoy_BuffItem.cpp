//---------------------------------------------------------------------------
//!	@file	PoyPoy_BuffItem.cpp
//! @brief	PoyPoy_BuffItem
//---------------------------------------------------------------------------

#include "PoyPoy_BuffItem.h"
#include "PoyPoy_Player.h"
#include "Component/ComponentThrowable.h"
#include <System/Scene.h>
#include <System/Component/ComponentCollision.h>
#include <System/Component/ComponentModel.h>

namespace PoyPoy {
//! @brief 初期化
//! @return 初期化終了
bool BuffItem::Init()
{
    // コリジョン生成前に設計値を設定する(派生クラスが数値を変える)
    InitSettings();

    Super::Init();

    SetName(EffectName());

    return true;
}

//! @brief 設計値の設定(基底クラス既定は移動速度1.5倍を10秒。派生クラスで上書きする)
void BuffItem::InitSettings()
{
    // 見た目の暫定版(専用モデルが用意できたら差し替える)
    SetModelPath("data/Game/Models/Object/Crystal.mv1");
    SetModelScale(0.2f);
    SetRadius(4.0f);
    SetHeight(14.0f);
    SetThrowSpeed(45.0f);
    SetWeight(15.0f);
    SetBounce(0.2f);
    SetFriction(18.0f);
    SetGravityAccel(98.0f);
    SetSpin(180.0f);
    SetDamage(0.0f);        // 直撃ではダメージを与えず、効果を適用する
    SetLiftTime(-1.0f);
}

//! @brief 当たり処理(プレイヤーへ当たったら効果を適用してアイテムを消費する)
void BuffItem::OnHit(const ComponentCollision::HitInfo& hitInfo)
{
    __super::OnHit(hitInfo);

    if(applied_ || !hitInfo.hit_ || !hitInfo.collision_ || !hitInfo.hit_collision_)
        return;

    // 所持中は効果を発動しない(手に持ったまま自分へ当たり続けても発動しない)
    if(auto throwable = GetComponent<ComponentThrowable>(); throwable && throwable->IsHeld())
        return;

    // オーバーラップ(押し合い跳过)でも効果は発動する。
    // 投げ物とプレイヤーは物理的に押し合わないようオーバーラップ設定になるため、
    // ここで除外すると効果を一切発動できない。除外するのは所持中だけ

    // プレイヤーへの当たりだけ効果発動(ほかのキャラは素通り)
    auto player = std::dynamic_pointer_cast<Player>(hitInfo.hit_collision_->GetOwnerPtr());
    if(!player)
        return;

    if(!ApplyTo(player))
        return;

    applied_ = true;
    HideAsCarrier();
}

//! @brief 効果の適用(既定実装は能力値を amount_ で書き換え元値を控える)
bool BuffItem::ApplyTo(const CharacterPtr& character)
{
    if(!character || !character->IsAlive())
        return false;

    // 変更前の値を控えておき、時間切れで元へ戻す
    switch(stat_) {
    case BuffStat::HP:
        restore_value_ = character->GetHP();
        character->SetHP(Modify(restore_value_));
        break;
    case BuffStat::POWER:
        restore_value_ = character->GetPower();
        character->SetPower(Modify(restore_value_));
        break;
    case BuffStat::MOVE_SPEED:
        restore_value_ = character->GetMoveSpeed();
        character->SetMoveSpeed(Modify(restore_value_));
        break;
    }
    StartDuration(character);
    return true;
}

//! @brief 効果の持続を開始する(カスタム ApplyTo 専用)
void BuffItem::StartDuration(const CharacterPtr& character)
{
    target_        = std::weak_ptr<Character>(character);
    buff_end_time_ = Scene::GetTime() + duration_;    // シーン時間(秒)で消滅時刻を決める
}

//! @brief 現在の値を設定値で書き換える
float BuffItem::Modify(float value) const
{
    if(is_add_)
        return value + (is_debuff_ ? -amount_ : amount_);    // 加算(デバフは減算)
    return value * amount_;                                  // 倍率(バフ1.5倍/デバフ0.5倍など)
}

//! @brief 効果発動時にアイテムを invisible な運搬体にする(モデル除去・コリジョンと投げ物挙動を無効化)
void BuffItem::HideAsCarrier()
{
    if(auto mdl = GetComponent<ComponentModel>())
        RemoveComponent(mdl);
    // 当たったあとのアイテムはもう何にも当たりたくないし動かない
    if(auto col = GetComponent<ComponentCollision>())
        col->SetStatus(Component::StatusBit::Enable, false);
    if(auto throwable = GetComponent<ComponentThrowable>())
        throwable->SetStatus(Component::StatusBit::Enable, false);
}

//! @brief 更新(効果発動後は持続時間を管理し、時間切れで能力値を戻して消える)
void BuffItem::Update()
{
    __super::Update();

    if(!applied_)
        return;

    if(Scene::GetTime() < buff_end_time_)
        return;

    // 時間切れで適用した効果を元へ戻す(キャラが消えていたら戻さない。
    // duration_=0 の回復アイテムなど「一回限り」の効果は元へ戻さない)
    if(duration_ > 0.0f) {
        if(auto target = target_.lock())
            Restore(target);
    }

    // 効果終わりでアイテム消滅
    Scene::Object::Release(SharedThis());
}

//! @brief 効果終了時に適用した効果を元へ戻す(既定実装は stat_ の能力値復元)
void BuffItem::Restore(const CharacterPtr& target)
{
    switch(stat_) {
    case BuffStat::HP:
        target->SetHP(restore_value_);
        break;
    case BuffStat::POWER:
        target->SetPower(restore_value_);
        break;
    case BuffStat::MOVE_SPEED:
        target->SetMoveSpeed(restore_value_);
        break;
    }
}

}    // namespace PoyPoy
