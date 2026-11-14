//---------------------------------------------------------------------------
//!	@file	PoyPoy_Player.h
//! @brief	PoyPoy_Player
//---------------------------------------------------------------------------
#include <Game/PoyPoy_Character.h>

namespace PoyPoy {
USING_PTR(Player);
class Player : public Character
{
public:
    BP_OBJECT_DECL(Player, "PoyPoy::Player");

    //! @brief 初期化
    //! @return 初期化終了
    bool Init() override;

    void Update() override;

    void OnHit(const ComponentCollision::HitInfo& hitInfo);

    //! @brief 移動入力の方向反転設定(押した方向と逆に動く。反転アイテム等の操作混乱デバフ用)
    void SetInputReversed(bool reversed);

    //! @brief 移動入力の方向が反転中か
    bool IsInputReversed() const;

private:
};
}    // namespace PoyPoy

