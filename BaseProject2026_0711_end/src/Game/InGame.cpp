//---------------------------------------------------------------------------
//!	@file	InGame.cpp
//! @brief	InGame
//---------------------------------------------------------------------------
#include "InGame.h"
#include "PoyPoy_GameOver.h"

#include "PoyPoy_Player.h"
#include "PoyPoy_Dummy.h"
#include "PoyPoy_Ground.h"
#include "PoyPoy_Camera.h"
#include "PoyPoy_Crystal.h"
#include "PoyPoy_Spawner.h"

namespace PoyPoy {

//! @brief 初期化
//! @return 初期化済み
bool InGame::Init()
{
    // 最初に1回動作する
    // ただし trueを返さなければ Initに何回も来る仕様。

    Scene::Object::Create<Ground>();

    Scene::Object::Create<Player>()->SetTranslate(float3(0, 5.0f, 0));

    // 移動機能を持たないダミーのプレイヤー
    Scene::Object::Create<Dummy>();

    Scene::Object::Create<Camera>();

    Scene::Object::Create<Crystal>()->SetTranslate(float3(0, 50.0f, 0));

    // 飛び回ってランダムに投げ物を生成するスポーン源
    Scene::Object::Create<Spawner>()->SetTranslate(float3(0, 25.0f, 0));

    return true;
}

void InGame::Update()
{
}

}    // namespace PoyPoy
