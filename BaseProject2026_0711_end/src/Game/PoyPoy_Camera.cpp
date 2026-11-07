//---------------------------------------------------------------------------
//!	@file	PoyPoy_Camera.cpp
//! @brief	PoyPoy_Camera
//---------------------------------------------------------------------------

#include "PoyPoy_Camera.h"
#include "InGame.h"
#include "Component/ComponentCameraTopDown.h"

namespace PoyPoy {
//! @brief 初期化
//! @return 初期化終了
bool Camera::Init()
{
    Super::Init();

    SetName("Camera");

    // 上から見下ろす固定構図(マウス・キーで回す方式はやめた)
    AddComponent<ComponentCamera>()    //
        ->SetPerspective(45.0f)
        ->SetNearFar(1.0f, 1000.0f);

    // 見下ろしの構図は ComponentCameraTopDown が毎フレーム焦点から組み立てる
    AddComponent<ComponentCameraTopDown>()           //
        ->SetFocusCenter({0.0f, 0.0f, 0.0f})         // 見下ろす中心
        ->SetHeight(140.0f)                          // 焦点からのカメラ高さ
        ->SetPitch(40.0f)                            // 水平から40度の斜め上から見る見下ろし
        ->SetScreenYaw(180.0f)                         // 画面の上=世界+Z(従来の左右向きを維持)
        ->SetFollowName("Player")                    // 将来キャラクター位置へ寄せる対象
        ->SetFollowWeight(0.0f);                     // 0=完全固定。寄せるなら 0.0..1.0 を上げる

    return true;
}
}    // namespace PoyPoy
