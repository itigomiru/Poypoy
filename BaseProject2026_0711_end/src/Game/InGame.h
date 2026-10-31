//---------------------------------------------------------------------------
//!	@file	InGame.h
//! @brief	InGame
//---------------------------------------------------------------------------
#include <System/Scene.h>
#include "PoyPoy_UI.h"

namespace PoyPoy {

class InGame : public Scene::Base
{
public:
    static constexpr int MAX_ENEMIES = 5;

    BP_CLASS_DECL(InGame, u8"PoyPoy::InGame");

    //! @brief 初期化
    //! @return 初期化済み
    bool Init() override;

    void Update() override;

    void LateDraw() override;

    void AddDeadEnemy();

private:
    void createEnemy();

    int enemy_dead_count_ = 0;

    //PoyPoy_UI ui_;
};

}    // namespace PoyPoy
