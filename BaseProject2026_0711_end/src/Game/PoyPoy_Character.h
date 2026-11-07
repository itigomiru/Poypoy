//---------------------------------------------------------------------------
//!	@file	PoyPoy_Character.h
//! @brief	PoyPoy_Character
//---------------------------------------------------------------------------
#pragma once
#include <System/Object.h>

namespace PoyPoy {
USING_PTR(Character);

//! @brief キャラごとに HP・パワー・素早さ を持たせるためのキャラクター共通ベースクラス。
//!        Player や Dummy など キャラクター扱いされるオブジェクトの基底として使う。
class Character : public Object
{
public:
    BP_OBJECT_DECL(Character, "PoyPoy::Character");

    //! @brief 初期化
    //! @return 初期化終了
    bool Init() override;

    //! @brief 最大HP取得
    inline float GetMaxHP() const { return max_hp_; }

    //! @brief 最大HP設定(現在HPが新しい最大を超える場合は切り詰める)
    CharacterPtr SetMaxHP(float hp);

    //! @brief 現在HP取得
    inline float GetHP() const { return hp_; }

    //! @brief 現在HP設定(0以上・最大HP以内に収める)
    CharacterPtr SetHP(float hp);

    //! @brief 生存しているか
    inline bool IsAlive() const { return hp_ > 0.0f; }

    //! @brief パワー(攻撃力)取得
    inline float GetPower() const { return power_; }

    //! @brief パワー(攻撃力)設定
    CharacterPtr SetPower(float power);

    //! @brief 素早さ(移動速度)取得
    inline float GetMoveSpeed() const { return move_speed_; }

    //! @brief 素早さ(移動速度)設定
    CharacterPtr SetMoveSpeed(float speed);

protected:
    float max_hp_     = 100.0f;   //!< 最大HP
    float hp_         = 100.0f;   //!< 現在HP
    float power_      = 10.0f;    //!< パワー(攻撃力)
    float move_speed_ = 0.3f;     //!< 素早さ(移動速度)
};
}    // namespace PoyPoy
