//---------------------------------------------------------------------------
//!	@file	PoyPoy_SpeedUpItem.h
//! @brief	PoyPoy_SpeedUpItem
//---------------------------------------------------------------------------
#pragma once
#include <Game/PoyPoy_BuffItem.h>

namespace PoyPoy {
USING_PTR(SpeedUpItem);

//! @brief 足速バフアイテム。
//!        プレイヤーに当たると、持続時間ぶん素早さ(移動速度)が1.8倍になる。
//!        移動はキャラクターの素早さステータスに追随しているので、効果が足に効く。
class SpeedUpItem : public BuffItem
{
public:
    BP_OBJECT_DECL(SpeedUpItem, "PoyPoy::SpeedUpItem");

    //! @brief 効果名
    const char* EffectName() const override { return "SpeedUpItem"; }

protected:
    //! @brief 設計値の設定(素早さ1.8倍を10秒)
    void InitSettings() override;
};
}    // namespace PoyPoy
