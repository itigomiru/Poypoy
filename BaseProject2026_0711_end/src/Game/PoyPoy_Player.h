//---------------------------------------------------------------------------
//!	@file	PoyPoy_Player.h
//! @brief	PoyPoy_Player
//---------------------------------------------------------------------------
#include <System/Scene.h>

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
};
}    // namespace PoyPoy
