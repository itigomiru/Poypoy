//---------------------------------------------------------------------------
//! @file   ComponentCrouch.h
//! @brief  しゃがみコンポーネント(キー長押しでしゃがみ状態を持つ)
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>
#include <System/Component/ComponentModel.h>
#include <System/VectorMath.h>

USING_PTR(ComponentCrouch);

//! @brief キー長押し中にしゃがみ状態を持つコンポーネント。
//!        しゃがみ中はノックバックを無効化し、ダメージを damage_scale_ 倍にする。
//!        ComponentKnockback と、これから作る爆風の処理にしゃがみ状態を提供する。
//!        Player などのオーナーに AddComponent して使う。
class ComponentCrouch : public Component
{
public:
    BP_COMPONENT_DECL(ComponentCrouch, u8"しゃがみ");

    void Init() override;

    void Update() override;

    void GUI() override;

    //! @brief しゃがみ中かを取得(爆風ノックバック無効化・ダメージ軽減の判定に使う)
    bool IsCrouching() const { return crouching_; }

    //! @brief しゃがみキーの設定
    //! @param key キーID
    void SetCrouchKey(int key) { key_crouch_ = key; }
    //! @brief しゃがみキーの取得
    //! @return キーID
    int GetCrouchKey() const { return key_crouch_; }

    //! @brief しゃがみ中の移動速度倍率の設定
    //! @param scale 倍率(0.0-1.0)
    void SetMoveScale(float scale) { move_scale_ = scale; }
    //! @brief しゃがみ中の移動速度倍率の取得
    //! @return 倍率
    float GetMoveScale() const { return move_scale_; }

    //! @brief しゃがみ中のダメージ倍率の設定(ノックバックと爆風処理が使う)
    //! @param scale 倍率(0.0-1.0)
    void SetDamageScale(float scale) { damage_scale_ = scale; }
    //! @brief しゃがみ中のダメージ倍率の取得
    //! @return 倍率
    float GetDamageScale() const { return damage_scale_; }

    //! @brief しゃがみ中のモデル縮小率の設定
    //! @param scale 縮小率(0.1-1.0)
    void SetCrouchScale(float scale) { crouch_scale_ = scale; }
    //! @brief しゃがみ中のモデル縮小率の取得
    //! @return 縮小率
    float GetCrouchScale() const { return crouch_scale_; }

private:
    ComponentModelWeakPtr mdl_{};                     //!< しゃがみ縮小制御用モデル(循環参照回避のためweak)
    float3 base_scale_ = {1.0f, 1.0f, 1.0f};          //!< Init時点のモデルスケール(復帰用)

    bool  crouching_    = false;                      //!< しゃがみ中か
    int   key_crouch_   = KEY_INPUT_C;                //!< しゃがみキー
    float move_scale_   = 0.5f;                       //!< しゃがみ中の移動速度倍率
    float damage_scale_ = 0.5f;                       //!< しゃがみ中のダメージ倍率
    float crouch_scale_ = 0.65f;                      //!< しゃがみ中のモデル縮小率

    //! @brief しゃがみ状態の設定とモデルの縮小/復帰
    //! @param crouching しゃがむか
    void SetCrouching(bool crouching);

    //--------------------------------------------------------------------
    //! @name Cereal処理
    //--------------------------------------------------------------------
    //@{

    //! @brief セーブ
    // @param arc アーカイバ
    // @param ver バージョン
    CEREAL_SAVELOAD(arc, ver)
    {
        arc(CEREAL_NVP(key_crouch_),
            CEREAL_NVP(move_scale_),
            CEREAL_NVP(damage_scale_),
            CEREAL_NVP(crouch_scale_));
        arc(cereal::make_nvp("Component", cereal::base_class<Component>(this)));
    }
    //@}
};

CEREAL_CLASS_VERSION(ComponentCrouch, 1);
