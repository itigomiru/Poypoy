//---------------------------------------------------------------------------
//!	@file	PoyPoy_UI.cpp
//! @brief	PoyPoy_UI
//---------------------------------------------------------------------------
#include "PoyPoy_UI.h"
#include "InGame.h"

#include <System/Scene.h>
#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentObjectController.h>
#include <System/Component/ComponentCollisionCapsule.h>
#include <System/Component/ComponentCollisionModel.h>
#include <System/Component/ComponentSpringArm.h>

namespace PoyPoy {

bool PoyPoy_UI::Init()
{
    // 初期化は完了しているとする
    return true;
}

void PoyPoy_UI::Update()
{
    
}   

void PoyPoy_UI::LateDraw()
{
    float ADJUST_X = (WINDOW_W / 1280.0f);
    float ADJUST_Y = (WINDOW_H / 720.0f);

    SetFontSize(static_cast<int>(UI_size_ * ADJUST_X));

    DrawFormatString(static_cast<int>(UI_x_ * ADJUST_X), static_cast<int>(UI_y_ * ADJUST_Y), GetColor(255, 255, 255), "UI");
}

void PoyPoy_UI::GUI()
{
    ImGui::DragInt("UILeft", &UI_x_);
    ImGui::DragInt("UITop", &UI_y_);
    ImGui::DragInt("UISize", &UI_size_);
}

}    // namespace PoyPoy
