//---------------------------------------------------------------------------
//!	@file	InGame.cpp
//! @brief	InGame
//---------------------------------------------------------------------------
#include "InGame.h"
#include "PoyPoy_GameOver.h"

#include "PoyPoy_Player.h"
#include "PoyPoy_Ground.h"
#include "PoyPoy_Camera.h"
#include "PoyPoy_Enemy.h"
#include "PoyPoy_ThrowableObject.h"

namespace PoyPoy {

//! @brief 初期化
//! @return 初期化済み
bool InGame::Init()
{
    // 最初に1回動作する
    // ただし trueを返さなければ Initに何回も来る仕様。

    Scene::Object::Create<Ground>();

    Scene::Object::Create<Player>();

    Scene::Object::Create<Camera>();

    Scene::Object::Create<ThrowableObject>();

    return true;
}

void InGame::Update()
{
    printfDx("\nDEAD ENEMY: %d", enemy_dead_count_);

    if(enemy_dead_count_ >= MAX_ENEMIES) {
        //createEnemy();
        Scene::Change(Scene::GetScene<PoyPoy_GameOver>());

        enemy_dead_count_ = 0;
    }
}

void InGame::AddDeadEnemy()
{
    enemy_dead_count_++;
}

}    // namespace PoyPoy
