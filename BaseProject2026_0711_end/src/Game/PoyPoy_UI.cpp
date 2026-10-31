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
    UI_Box = LoadGraph("data/Game/Images/UI_BOX.png");
    // 初期化は完了しているとする
    return true;
}

void PoyPoy_UI::Update()
{
}

void PoyPoy_UI::Draw()
{
    float ADJUST_X = (WINDOW_W / 1280.0f);
    float ADJUST_Y = (WINDOW_H / 720.0f);

    SetFontSize(static_cast<int>(UI_size_ * ADJUST_X));

    // DrawBox(100, 100, 300, 300, GetColor(255, 0, 0), TRUE);

    for(int i = 0; i < Player_num_; i++) {
        DrawGraph(UI_x_ + i * 300, UI_y_, UI_Box, TRUE);
    }
}

void PoyPoy_UI::GUI()
{
    ImGui::DragInt("UILeft", &UI_x_);
    ImGui::DragInt("UITop", &UI_y_);
    ImGui::DragInt("UISize", &UI_size_);
}

}    // namespace PoyPoy
