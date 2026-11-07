//---------------------------------------------------------------------------
//!	@file	PoyPoy_Character.cpp
//! @brief	PoyPoy_Character
//---------------------------------------------------------------------------

#include "PoyPoy_Character.h"

namespace PoyPoy {
//! @brief 初期化
//! @return 初期化終了
bool Character::Init()
{
    hp_ = max_hp_;

    return Super::Init();
}

//! @brief 最大HP設定
//! @param hp 新しい最大HP
//! @return 自分の共有ポインタ
CharacterPtr Character::SetMaxHP(float hp)
{
    max_hp_ = hp;

    if(hp_ > max_hp_)
        hp_ = max_hp_;

    return std::static_pointer_cast<Character>(SharedThis());
}

//! @brief 現在HP設定
//! @param hp 新しい現在HP
//! @return 自分の共有ポインタ
CharacterPtr Character::SetHP(float hp)
{
    if(hp < 0.0f)
        hp_ = 0.0f;
    else if(hp > max_hp_)
        hp_ = max_hp_;
    else
        hp_ = hp;

    return std::static_pointer_cast<Character>(SharedThis());
}

//! @brief パワー(攻撃力)設定
//! @param power 新しいパワー
//! @return 自分の共有ポインタ
CharacterPtr Character::SetPower(float power)
{
    power_ = power;

    return std::static_pointer_cast<Character>(SharedThis());
}

//! @brief 素早さ(移動速度)設定
//! @param speed 新しい移動速度
//! @return 自分の共有ポインタ
CharacterPtr Character::SetMoveSpeed(float speed)
{
    move_speed_ = speed;

    return std::static_pointer_cast<Character>(SharedThis());
}

}    // namespace PoyPoy
