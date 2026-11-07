//---------------------------------------------------------------------------
//!	@file	PoyPoy_Crystal.h
//! @brief	PoyPoy_Crystal
//---------------------------------------------------------------------------
#pragma once
#include <Game/PoyPoy_ThrowableObject.h>

namespace PoyPoy {
USING_PTR(Crystal);
class Crystal : public ThrowableObject
{
public:
    BP_OBJECT_DECL(Crystal, "PoyPoy::Crystal");

    //! @brief 初期化
    //! @return 初期化終了
    bool Init() override;
};
}    // namespace PoyPoy
