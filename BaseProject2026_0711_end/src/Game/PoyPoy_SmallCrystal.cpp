//---------------------------------------------------------------------------
//!	@file	PoyPoy_SmallCrystal.cpp
//! @brief	PoyPoy_SmallCrystal
//---------------------------------------------------------------------------

#include "PoyPoy_SmallCrystal.h"

namespace PoyPoy {
//! @brief 初期化
//! @return 初期化終了
bool SmallCrystal::Init()
{
    // 軽く小さくてよく跳ねる水晶。軽いぶん軽く飛んで当たりやすい
    SetModelPath("data/Game/Models/Object/SmallCrystal.mv1");
    SetModelScale(0.15f);
    SetRadius(3.0f);
    SetHeight(12.0f);
    SetThrowSpeed(60.0f);
    SetWeight(10.0f);
    SetBounce(0.4f);
    SetFriction(10.0f);
    SetGravityAccel(98.0f);
    SetSpin(260.0f);
    SetDamage(5.0f);
    SetLiftTime(6.0f);    // 軽いぶん長く持てる(上限値は ComponentPickUp 側が実装するまで未反映)

    Super::Init();

    SetName("SmallCrystal");

    return true;
}

}    // namespace PoyPoy
