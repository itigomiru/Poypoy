#pragma once
//---------------------------------------------------------------------------
//!	@file	ComponentPlayerInput.cpp
//! @brief	プレイヤー入力
//---------------------------------------------------------------------------
#include <Game/Component/ComponentPlayerInput.h>

void ComponentPlayerInput::Init()
{
    __super::Init();

    SetName<Component>("Player Input");

    control_type_ = ControlType::Player;
}

//---------------------------------------------------------------------------
// 入力検証(物理入力)
//---------------------------------------------------------------------------
bool ComponentPlayerInput::IsKey(int keyID) const
{
    return Input::IsKey(keyID);
}

bool ComponentPlayerInput::IsKeyDown(int keyID) const
{
    return Input::IsKeyDown(keyID);
}

bool ComponentPlayerInput::IsKeyUp(int keyID) const
{
    return Input::IsKeyUp(keyID);
}

bool ComponentPlayerInput::IsKeyOn(int keyID) const
{
    return Input::IsKeyOn(keyID);
}

bool ComponentPlayerInput::IsKeyRelease(int keyID) const
{
    return Input::IsKeyRelease(keyID);
}

bool ComponentPlayerInput::IsKeyRepeat(int keyID, u32 frame) const
{
    return Input::IsKeyRepeat(keyID, frame);
}

CEREAL_REGISTER_TYPE(ComponentPlayerInput)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentPlayerInput)
