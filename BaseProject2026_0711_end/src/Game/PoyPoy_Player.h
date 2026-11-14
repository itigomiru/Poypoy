//---------------------------------------------------------------------------
//!	@file	PoyPoy_Player.h
//! @brief	PoyPoy_Player
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include <Game/Component/ComponentInput.h>

namespace PoyPoy {
USING_PTR(Player);
class Player : public Object
{
public:
    BP_OBJECT_DECL(Player, "PoyPoy::Player");

    //! @brief 初期化
    //! @return 初期化終了
    bool Init() override;

    void Update() override;

    //--------------------------------------------------------------------
    //! @name 操作タイプ
    //! @details キャラ本体は操作タイプを持ちません。
    //!          入力ソースを切り替えるだけでPlayer・AIのどちらでも動きます。
    //@{

    //! @brief 操作タイプを設定します(入力ソースを切り替えます)
    PlayerPtr SetControlType(ControlType type);

    //! @brief 現在の操作タイプを取得します
    ControlType GetControlType() const;

    //! @brief 入力ソースを取得します(未設定時は既定の物理入力)
    ComponentInputPtr GetInput() const;

    //@}

    void OnHit(const ComponentCollision::HitInfo& hitInfo);

private:
};
}    // namespace PoyPoy
