//---------------------------------------------------------------------------
//!	@file	PoyPoy_PowerUpItem.cpp
//! @brief	PoyPoy_PowerUpItem
//---------------------------------------------------------------------------

#include "PoyPoy_PowerUpItem.h"

namespace PoyPoy {
//! @brief 設計値の設定(パワー1.5倍を10秒。時間切れで元値へ自動で戻る)
void PowerUpItem::InitSettings()
{
    Super::InitSettings();

    stat_      = BuffStat::POWER;
    amount_    = 1.5f;        // 倍率(元値×1.5)
    is_add_    = false;
    is_debuff_ = false;
    duration_  = 10.0f;
}

}    // namespace PoyPoy
