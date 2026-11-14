//---------------------------------------------------------------------------
//!	@file	PoyPoy_SpeedUpItem.cpp
//! @brief	PoyPoy_SpeedUpItem
//---------------------------------------------------------------------------

#include "PoyPoy_SpeedUpItem.h"

namespace PoyPoy {
//! @brief 設計値の設定(素早さ1.8倍を10秒。時間切れで元値へ自動で戻る)
void SpeedUpItem::InitSettings()
{
    Super::InitSettings();

    stat_      = BuffStat::MOVE_SPEED;
    amount_    = 1.8f;        // 倍率(元値×1.8)
    is_add_    = false;
    is_debuff_ = false;
    duration_  = 10.0f;
}

}    // namespace PoyPoy
