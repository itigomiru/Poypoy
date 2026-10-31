//---------------------------------------------------------------------------
//! @file   ComponentKnockback.h
//! @brief  ノックバックコンポーネント(投げ物を体躯で受け止めてノックバックする)
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>
#include <System/Component/ComponentCollision.h>
#include <System/Component/ComponentCollisionCapsule.h>
#include <System/Component/ComponentModel.h>

USING_PTR(ComponentKnockback);

//! @brief 体躯と同じ形状の受け用コリジョンを自分で持ち、THROWABLEが当たると
//!        ノックバック→無敵時間(点滅)→通常復帰を行うコンポーネント。
//!        Player/DummyなどのオーナーにAddComponentして使う。
class ComponentKnockback : public Component
{
public:
    BP_COMPONENT_DECL(ComponentKnockback, u8"ノックバック");

    void Init() override;

    void Update() override;

    //! @brief ノックバック中かを取得(ノックバック中の操作不可の判定に使う)
    bool IsKnockingBack() const { return knocking_; }

    //! @brief ノックバック中または無敵時間中かを取得(当たり判定なしの判定に使う)
    bool IsInvincible() const { return knocking_ || invincible_left_ > 0.0f; }

    //! @brief ノックバックの速度(一律の強さ)の設定
    //! @param speed 速度
    void SetKnockSpeed(float speed) { knock_speed_ = speed; }
    //! @brief ノックバックの速度の取得
    //! @return 速度
    float GetKnockSpeed() const { return knock_speed_; }

    //! @brief ノックバック終了後の無敵時間の設定
    //! @param time 無敵時間(秒)
    void SetInvincibleTime(float time) { invincible_time_ = time; }
    //! @brief 無敵時間の取得
    //! @return 無敵時間(秒)
    float GetInvincibleTime() const { return invincible_time_; }

    //! @brief 受け用コリジョンの形状の設定(Init前に設定すること)
    //! @param radius 半径
    //! @param height 高さ
    //! @param translate オフセット
    void SetTargetShape(float radius, float height, const float3& translate)
    {
        target_radius_    = radius;
        target_height_    = height;
        target_translate_ = translate;
    }

    //! @brief 受け用コリジョンから呼ばれる当たり処理
    //! @param hitInfo 当たった情報
    void OnKnockHit(const ComponentCollision::HitInfo& hitInfo);

private:
    ComponentCollisionCapsuleWeakPtr target_{};    //!< 自分が追加した受け用コリジョン(循環参照回避のためweak)
    ComponentModelWeakPtr            mdl_{};       //!< 無敵中の点滅制御用モデル(循環参照回避のためweak)

    bool   knocking_        = false;                    //!< ノックバック中か
    float  invincible_left_ = 0.0f;                     //!< ノックバック終了後の無敵時間の残り(秒)
    float3 knock_velocity_  = {0.0f, 0.0f, 0.0f};       //!< ノックバックの速度
    float  knock_speed_     = 40.0f;                    //!< ノックバックの速度(一律の強さ)
    float  knock_friction_  = 6.0f;                     //!< ノックバックの摩擦(毎秒の減速率)
    float  invincible_time_ = 2.0f;                     //!< ノックバック終了後の無敵時間(秒)
    bool   flash_on_        = true;                     //!< 点滅の表示状態
    float  flash_left_      = 0.0f;                     //!< 次の点滅切り替えまでの残り(秒)
    float  flash_interval_  = 0.12f;                    //!< 点滅の切り替え間隔(秒)

    float  target_radius_    = 5.0f;                    //!< 受け用コリジョンの半径
    float  target_height_    = 21.0f;                   //!< 受け用コリジョンの高さ
    float3 target_translate_ = {0.0f, 0.0f, 0.0f};      //!< 受け用コリジョンのオフセット
    float  target_mass_      = 50.0f;                   //!< 受け用コリジョンの質量(投げ物より押し負けない重さ)

    //--------------------------------------------------------------------
    //! @name Cereal処理
    //--------------------------------------------------------------------
    //@{

    //! @brief セーブ
    // @param arc アーカイバ
    // @param ver バージョン
    CEREAL_SAVELOAD(arc, ver)
    {
        arc(CEREAL_NVP(knock_speed_),
            CEREAL_NVP(knock_friction_),
            CEREAL_NVP(invincible_time_),
            CEREAL_NVP(flash_interval_),
            CEREAL_NVP(target_radius_),
            CEREAL_NVP(target_height_),
            CEREAL_NVP(target_mass_));
        arc(cereal::make_nvp("Component", cereal::base_class<Component>(this)));
    }
    //@}
};
