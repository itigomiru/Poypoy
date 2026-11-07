//---------------------------------------------------------------------------
//! @file   ComponentDodge.h
//! @brief  前転回避コンポーネント(キー押下で向いている方向へ前転する)
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>
#include <System/Component/ComponentModel.h>
#include <System/VectorMath.h>

USING_PTR(ComponentDodge);

//! @brief 回避キー押下時に、モデルの向いている方向へ前転するコンポーネント。
//!        前転中は ComponentStateIdleWalk の移動入力を抑止し、前転側の移動に任せる。
//!        前転中に再度押すと次の前転を予約し、前転終了直後に連続で開始する。
//!        押してから設定時間(既定0.1秒)は無敵時間(点滅なし)になる。
//!        ノックバックを食らったら前転を中止してノックバックに移動を譲る。
//!        Player などのオーナーに AddComponent して使う。
class ComponentDodge : public Component
{
public:
    BP_COMPONENT_DECL(ComponentDodge, u8"前転回避");

    void Init() override;

    void Update() override;

    void GUI() override;

    //! @brief 前転中かを取得(IdleWalk側で移動入力を抑止する判定に使う)
    bool IsDodging() const { return dodging_; }

    //! @brief 回避キーの設定
    //! @param key キーID
    void SetDodgeKey(int key) { key_dodge_ = key; }
    //! @brief 回避キーの取得
    //! @return キーID
    int GetDodgeKey() const { return key_dodge_; }

    //! @brief 前転の移動速度の設定
    //! @param speed 速度
    void SetRollSpeed(float speed) { roll_speed_ = speed; }
    //! @brief 前転の移動速度の取得
    //! @return 速度
    float GetRollSpeed() const { return roll_speed_; }

    //! @brief 前転の継続時間の設定
    //! @param time 継続時間(秒)
    void SetRollTime(float time) { roll_time_ = time; }
    //! @brief 前転の継続時間の取得
    //! @return 継続時間(秒)
    float GetRollTime() const { return roll_time_; }

    //! @brief 前転開始直後の無敵時間の設定
    //! @param time 無敵時間(秒)
    void SetInvincibleTime(float time) { invincible_time_ = time; }
    //! @brief 前転開始直後の無敵時間の取得
    //! @return 無敵時間(秒)
    float GetInvincibleTime() const { return invincible_time_; }

private:
    ComponentModelWeakPtr mdl_{};                     //!< 前転開始時の正面取得用モデル(循環参照回避のためweak)

    bool   dodging_     = false;                      //!< 前転中か
    bool   dodge_queued_ = false;                     //!< 前転中に再度押されたか(前転終了直後に次の前転を開始する)
    float  roll_left_   = 0.0f;                       //!< 前転の残り時間(秒)
    float3 roll_dir_    = {0.0f, 0.0f, -1.0f};        //!< 前転開始時に捕捉した正面方向
    int    key_dodge_   = KEY_INPUT_LSHIFT;           //!< 回避キー
    float  roll_speed_  = 0.8f;                       //!< 前転の移動速度
    float  roll_time_   = 0.3f;                       //!< 前転の継続時間(秒)
    float  invincible_time_ = 0.1f;                   //!< 前転開始直後に付与する無敵時間(秒)

    //! @brief 前転を開始できるか検証して開始する(ノックバック・ノックバック後の無敵時間中・しゃがみ中は開始しない)
    void TryStartDodge();

    //! @brief 前転を開始する(正面方向を捕捉して前転状態にする)
    void StartDodge();

    //--------------------------------------------------------------------
    //! @name Cereal処理
    //--------------------------------------------------------------------
    //@{

    //! @brief セーブ
    // @param arc アーカイバ
    // @param ver バージョン
    CEREAL_SAVELOAD(arc, ver)
    {
        arc(CEREAL_NVP(key_dodge_),
            CEREAL_NVP(roll_speed_),
            CEREAL_NVP(roll_time_),
            CEREAL_NVP(invincible_time_));
        arc(cereal::make_nvp("Component", cereal::base_class<Component>(this)));
    }
    //@}
};

CEREAL_CLASS_VERSION(ComponentDodge, 2);
