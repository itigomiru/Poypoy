//---------------------------------------------------------------------------
//!	@file	PoyPoy_FlipInputItem.cpp
//! @brief	PoyPoy_FlipInputItem
//---------------------------------------------------------------------------

#include "PoyPoy_FlipInputItem.h"
#include "PoyPoy_Player.h"

namespace PoyPoy {
//! @brief 設計値の設定(10秒持続するデバフ。見た目は基底の暫定モデルのまま)
void FlipInputItem::InitSettings()
{
    Super::InitSettings();

    SetWeight(12.0f);       // 少し軽くする
    SetLiftTime(3.0f);      // 持たれても早めに投げる

    duration_  = 10.0f;     // 持続時間(秒)
    is_debuff_ = true;      // デバフ
}

//! @brief 効果の適用(プレイヤーの移動入力方向反転を開始)
bool FlipInputItem::ApplyTo(const CharacterPtr& character)
{
    auto player = std::dynamic_pointer_cast<Player>(character);
    if(!player)
        return false;

    player->SetInputReversed(true);
    StartDuration(character);
    return true;
}

//! @brief 効果の終了(移動入力の方向反転を解除)
void FlipInputItem::Restore(const CharacterPtr& target)
{
    if(auto player = std::dynamic_pointer_cast<Player>(target))
        player->SetInputReversed(false);
}

}    // namespace PoyPoy
