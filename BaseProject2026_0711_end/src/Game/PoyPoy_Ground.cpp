//---------------------------------------------------------------------------
//!	@file	PoyPoy_Player.cpp
//! @brief	PoyPoy_Player
//---------------------------------------------------------------------------

#include "PoyPoy_Ground.h"
#include "InGame.h"

namespace PoyPoy {
//! @brief 初期化
//! @return 初期化終了
bool Ground::Init()
{
    Super::Init();

    SetName("Ground");

    //auto ground = Scene::Object::Create<Object>("Ground");
    AddComponent<ComponentModel>("data/Game/Models/Terrain/moon.mv1")->SetScaleAxisXYZ(0.005);
    AddComponent<ComponentCollisionModel>()->AttachToModel();
    AddComponent<ComponentPhysics>()->CreateMesh(GetComponent<ComponentModel>(), 1.0f);

    return true;
}
}    // namespace PoyPoy
