//---------------------------------------------------------------------------
//!	@file	PoyPoy_HealItem.h
//! @brief	PoyPoy_HealItem
//---------------------------------------------------------------------------
#pragma once
#include <Game/PoyPoy_BuffItem.h>

namespace PoyPoy {
USING_PTR(HealItem);

//! @brief 回復アイテム(一回限り)。
//!        プレイヤーに当たるとHPを+20回復する(最大HPまで。それ以上は増えない)。
//!        持続時間0の一回限り効果なので、消滅してもHPが元へ戻ることはない。
class HealItem : public BuffItem
{
public:
    BP_OBJECT_DECL(HealItem, "PoyPoy::HealItem");

    //! @brief 効果名
    const char* EffectName() const override { return "HealItem"; }

protected:
    //! @brief 設計値の設定(HPを+20回復)
    void InitSettings() override;
};
}    // namespace PoyPoy
