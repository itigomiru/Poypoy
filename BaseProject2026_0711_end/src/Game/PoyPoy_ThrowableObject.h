//---------------------------------------------------------------------------
//!	@file	PoyPoy_ThrowableObject.h
//! @brief	PoyPoy_ThrowableObject
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include <Game/PoyPoy_Throwable.h>

namespace PoyPoy {
USING_PTR(ThrowableObject);
class ThrowableObject : public Throwable
{
public:
    BP_OBJECT_DECL(ThrowableObject, "PoyPoy::ThrowableObject");

    //! @brief 初期化
    //! @return 初期化終了
    bool Init() override;

    void Update() override;

private:
};
}    // namespace PoyPoy
