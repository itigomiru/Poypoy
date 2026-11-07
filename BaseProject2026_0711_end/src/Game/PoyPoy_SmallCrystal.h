//---------------------------------------------------------------------------
//!	@file	PoyPoy_SmallCrystal.h
//! @brief	PoyPoy_SmallCrystal
//---------------------------------------------------------------------------
#pragma once
#include <Game/PoyPoy_ThrowableObject.h>

namespace PoyPoy {
USING_PTR(SmallCrystal);
class SmallCrystal : public ThrowableObject
{
public:
    BP_OBJECT_DECL(SmallCrystal, "PoyPoy::SmallCrystal");

    //! @brief 初期化
    //! @return 初期化終了
    bool Init() override;
};
}    // namespace PoyPoy
