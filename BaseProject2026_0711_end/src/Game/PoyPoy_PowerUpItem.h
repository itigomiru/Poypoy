//---------------------------------------------------------------------------
//!	@file	PoyPoy_PowerUpItem.h
//! @brief	PoyPoy_PowerUpItem
//---------------------------------------------------------------------------
#pragma once
#include <Game/PoyPoy_BuffItem.h>

namespace PoyPoy {
USING_PTR(PowerUpItem);

//! @brief パワーアップアイテム(バフ)。
//!        プレイヤーに当たると、持続時間ぶんパワーが1.5倍になる。
//!        パワーは「重さによる保持時間の上限(lift_time)」の伸び幅として反映されるため、
//!        効果中は岩石などの重いものを長く持てる(重いぶん短い保持時間の不利が和らぐ)。
class PowerUpItem : public BuffItem
{
public:
    BP_OBJECT_DECL(PowerUpItem, "PoyPoy::PowerUpItem");

    //! @brief 効果名
    const char* EffectName() const override { return "PowerUpItem"; }

protected:
    //! @brief 設計値の設定(パワー1.5倍を10秒)
    void InitSettings() override;
};
}    // namespace PoyPoy
