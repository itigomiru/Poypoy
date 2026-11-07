//---------------------------------------------------------------------------
//!	@file	PoyPoy_Crystal.cpp
//! @brief	PoyPoy_Crystal
//---------------------------------------------------------------------------

#include "PoyPoy_Crystal.h"

namespace PoyPoy {
//! @brief 初期化
//! @return 初期化終了
bool Crystal::Init()
{
    // クリスタル用の設定(Super::Init()の前に呼ぶ)
    SetModelPath("data/Game/Models/Object/Crystal.mv1");
    SetModelScale(0.2f);
    SetRadius(5.0f);
    SetHeight(38.0f);
    SetThrowSpeed(50.0f);
    SetWeight(20.0f);
    SetBounce(0.25f);
    SetFriction(15.0f);
    SetGravityAccel(98.0f);
    SetSpin(180.0f);
    SetDamage(10.0f);    // クリスタルの直撃ダメージ

    Super::Init();

    SetName("Crystal");

    return true;
}

}    // namespace PoyPoy
