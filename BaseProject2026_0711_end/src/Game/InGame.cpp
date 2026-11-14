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
#include "PoyPoy_ThrowableObject.h"
#include "PoyPoy_UI.h"
#include "PoyPoy_Crystal.h"
#include "PoyPoy_Spawner.h"
#include "PoyPoy_Bomb.h"
#include "PoyPoy_Rock.h"
#include "PoyPoy_Crate.h"
#include "PoyPoy_BuffItem.h"
#include "PoyPoy_FlipInputItem.h"
#include "PoyPoy_PowerUpItem.h"
#include "PoyPoy_SpeedUpItem.h"
#include "PoyPoy_HealItem.h"

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

    // 拾って投げると導火線で爆発する爆弾(近接範囲ダメージ)
    Scene::Object::Create<Bomb>()->SetTranslate(float3(8.0f, 5.0f, 0));

    // 小さい爆弾(爆風が小さく軽い。爆風がボムに当たると誘爆する)
    Scene::Object::Create<SmallBomb>()->SetTranslate(float3(12.0f, 3.0f, 0));

    // 岩石の投げ物(スフィアコリジョン・重くて転がらない)
    Scene::Object::Create<Rock>()->SetTranslate(float3(16.0f, 5.0f, 0));

    // 投げると当たった瞬間に壊れて中身(Crystal)が出てくる木箱
    Scene::Object::Create<Crate>()->SetTranslate(float3(20.0f, 5.0f, 0));

    // バフ・デバフアイテムの基底クラス(既定=移動速度1.5倍を10秒。当たったら適用される)
    Scene::Object::Create<BuffItem>()->SetTranslate(float3(24.0f, 5.0f, 0));

    // 当たると移動入力の前後左右が10秒反転するデバフアイテム
    Scene::Object::Create<FlipInputItem>()->SetTranslate(float3(28.0f, 5.0f, 0));

    // パワー1.5倍を10秒(保持時間の上限が伸びて重い物を長く持てる)バフアイテム
    Scene::Object::Create<PowerUpItem>()->SetTranslate(float3(32.0f, 5.0f, 0));

    // 素早さ1.8倍を10秒(足が速くなる)バフアイテム
    Scene::Object::Create<SpeedUpItem>()->SetTranslate(float3(36.0f, 5.0f, 0));

    // 当たるとHPを+20回復する(一回限り)アイテム
    Scene::Object::Create<HealItem>()->SetTranslate(float3(40.0f, 5.0f, 0));

    Scene::Object::Create<Camera>();

    Scene::Object::Create<Crystal>()->SetTranslate(float3(0, 50.0f, 0));

    // 飛び回ってランダムに投げ物を生成するスポーン源
    Scene::Object::Create<Spawner>()->SetTranslate(float3(0, 25.0f, 0));

    // ゲーム画面にUIを表示する
    Scene::Object::Create<PoyPoy_UI>();

    return true;
}

void InGame::Update()
{
}

}    // namespace PoyPoy
