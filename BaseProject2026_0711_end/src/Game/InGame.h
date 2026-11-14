//---------------------------------------------------------------------------
//!	@file	InGame.h
//! @brief	InGame
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include <Game/Component/ComponentInput.h>

namespace PoyPoy {

//! @brief 試合開始前のキャラクター設定
struct CharacterConfig
{
    float3      translate;    //!< 生成位置
    ControlType control;      //!< 操作タイプ(Player / AI)

    int attack_key = KEY_INPUT_SPACE;    //!< アクション(攻撃・持ち上げ)キー
    int defend_key = 0;                  //!< 防御キー(AIのみ。0で無効)
    int dodge_key  = 0;                  //!< 回避キー(AIのみ。0で無効)
    int skill_key  = 0;                  //!< スキルキー(AIのみ。0で無効)
};

class InGame : public Scene::Base
{
public:
    BP_CLASS_DECL(InGame, u8"PoyPoy::InGame");

    //! @brief 初期化
    //! @return 初期化済み
    bool Init() override;

    void Update() override;

private:
};

}    // namespace PoyPoy
