//---------------------------------------------------------------------------
//!	@file	InGame.cpp
//! @brief	InGame
//---------------------------------------------------------------------------
#include "InGame.h"
#include "PoyPoy_GameOver.h"

#include "PoyPoy_Player.h"
#include "PoyPoy_Ground.h"
#include "PoyPoy_Camera.h"
#include "PoyPoy_ThrowableObject.h"
#include <Game/Component/ComponentAIInput.h>

namespace PoyPoy {

namespace {

//! @brief 試合に参加するキャラクターの一覧
//! @details controlを変えるだけで、同じキャラクターをPlayer・AIのどちらにも割り当てられます。
//!          1体目はカメラが追う対象になるのでPlayer操作のキャラを先に置いてください。
//!          defend_key/dodge_key/skill_key は 0 で無効(対応する行動の状態側が未実装)。
const CharacterConfig characters[] = {
    {float3(-10.0f, 5.0f, 0.0f), ControlType::Player}, // 自機
    { float3(10.0f, 5.0f, 0.0f),     ControlType::AI}, // AI機
};

}    // namespace

//! @brief 初期化
//! @return 初期化済み
bool InGame::Init()
{
    // 最初に1回動作する
    // ただし trueを返さなければ Initに何回も来る仕様。

    Scene::Object::Create<Ground>();

    // キャラクターを生成し、設定された操作タイプに応じた入力を割り当てる
    PlayerPtr player_controlled = nullptr;
    for(auto& config : characters) {
        auto character = Scene::Object::Create<Player>();
        character->SetTranslate(config.translate);
        character->SetControlType(config.control);

        // アクションキーは入力ソース経由で渡す
        // AIの攻撃とPlayerの持ち上げ・投げは同じキーとして扱われる
        if(auto input = character->GetInput()) {
            input->SetActionKey(config.attack_key);

            if(auto ai = std::dynamic_pointer_cast<ComponentAIInput>(input))
                ai->SetActionKeys(config.attack_key, config.defend_key, config.dodge_key, config.skill_key);
        }

        if(config.control == ControlType::Player && !player_controlled)
            player_controlled = character;
    }

    Scene::Object::Create<Camera>();

    // カメラは人間が操作するキャラクターを追う
    if(player_controlled) {
        if(auto camera = Scene::Object::Get<Camera>(); camera) {
            if(auto arm = camera->GetComponent<ComponentSpringArm>())
                arm->SetSpringArmObject(player_controlled);
        }
    }

    Scene::Object::Create<ThrowableObject>()->SetTranslate(float3(0, 50.0f, 0));

    return true;
}

void InGame::Update()
{
}

}    // namespace PoyPoy
