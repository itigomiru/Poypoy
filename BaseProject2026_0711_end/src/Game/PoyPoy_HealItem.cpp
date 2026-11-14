//---------------------------------------------------------------------------
//!	@file	PoyPoy_HealItem.cpp
//! @brief	PoyPoy_HealItem
//---------------------------------------------------------------------------

#include "PoyPoy_HealItem.h"

namespace PoyPoy {
//! @brief 設計値の設定(HP+20の加算回復。持続0=一回限りで元へ戻さない)
void HealItem::InitSettings()
{
    Super::InitSettings();

    stat_      = BuffStat::HP;
    amount_    = 20.0f;       // 回復量(加算)
    is_add_    = true;        // HPは倍率ではなく加算
    is_debuff_ = false;
    duration_  = 0.0f;        // 0=一回限りの効果(適用後すぐ消える。時間切れでの元値戻しなし)
}

}    // namespace PoyPoy
