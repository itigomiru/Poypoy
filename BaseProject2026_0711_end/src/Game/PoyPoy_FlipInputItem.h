//---------------------------------------------------------------------------
//!	@file	PoyPoy_FlipInputItem.h
//! @brief	PoyPoy_FlipInputItem
//---------------------------------------------------------------------------
#pragma once
#include <Game/PoyPoy_BuffItem.h>

namespace PoyPoy {
USING_PTR(FlipInputItem);

//! @brief 操作反転アイテム(操作混乱デバフ)。
//!        プレイヤーに当たると、持続時間ぶん移動入力の前後左右が反転する(押した方向と逆に動く)。
//!        BuffItem のカスタム効果(ApplyTo/Restore オーバーライド)のサンプルも兼ねる。
class FlipInputItem : public BuffItem
{
public:
    BP_OBJECT_DECL(FlipInputItem, "PoyPoy::FlipInputItem");

    //! @brief 効果名
    const char* EffectName() const override { return "FlipInputItem"; }

protected:
    //! @brief 設計値の設定(効果内容は ApplyTo/Restore が担当するので持続時間だけ設定する)
    void InitSettings() override;

    //! @brief 効果の適用(プレイヤーの移動入力方向反転を開始)
    bool ApplyTo(const CharacterPtr& character) override;

    //! @brief 効果の終了(移動入力の方向反転を解除)
    void Restore(const CharacterPtr& target) override;
};
}    // namespace PoyPoy
