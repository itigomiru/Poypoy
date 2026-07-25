//---------------------------------------------------------------------------
//!	@file	PoyPoy_Camera.cpp
//! @brief	PoyPoy_Camera
//---------------------------------------------------------------------------

#include "PoyPoy_Camera.h"
#include "InGame.h"

namespace PoyPoy {
//! @brief 初期化
//! @return 初期化終了
bool Camera::Init()
{
    Super::Init();

    SetName("Camera");

    //auto camera = Scene::Object::Create<Object>("Camera");
    AddComponent<ComponentCamera>()    //
        ->SetPositionAndTarget({0, 20, -50}, {0, 10, 0});
    AddComponent<ComponentSpringArm>()->SetSpringArmObject("Player");

    return true;
}
}    // namespace PoyPoy
