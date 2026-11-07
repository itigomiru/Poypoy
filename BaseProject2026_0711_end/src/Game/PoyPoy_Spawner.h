//---------------------------------------------------------------------------
//!	@file	PoyPoy_Spawner.h
//! @brief	PoyPoy_Spawner
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include <System/Component/ComponentModel.h>
#include <functional>
#include <random>
#include <vector>

namespace PoyPoy {
USING_PTR(Spawner);

//! @brief 一定時間ごとに投げ物をランダム生成する、座標を持って飛び回るスポーン源。
//!        自身の周囲のランダムな地点へ移動し続けているので、動いているのが見える。
class Spawner : public Object
{
public:
    BP_OBJECT_DECL(Spawner, "PoyPoy::Spawner");

    //! @brief 初期化
    //! @return 初期化終了
    bool Init() override;

    //! @brief 更新(移動とスポーン実行)
    void Update() override;

    //! @brief 移動範囲の半径(初期値25.0f)
    void SetWanderRadius(float radius) { wander_radius_ = radius; }

    //! @brief 飛行の高さ(初期値25.0f)
    void SetWanderHeight(float height) { wander_height_ = height; }

    //! @brief 移動速度(初期値8.0f)
    void SetMoveSpeed(float speed) { move_speed_ = speed; }

    //! @brief スポーン間隔の範囲(秒)(初期値3.0f～8.0f)
    void SetSpawnInterval(float min_time, float max_time)
    {
        spawn_time_min_ = min_time;
        spawn_time_max_ = max_time;
    }

    //! @brief スポーン位置のずれの大きさ(初期値5.0f)
    void SetSpawnOffset(float offset) { spawn_offset_ = offset; }

private:
    //! @brief 移動先のランダム地点を作る
    float3 RandomTarget();

    //! @brief スポーン間隔のランダム値を作る
    float RandomInterval();

    //! @brief 投げ物を1つランダムな種類で生成する
    void SpawnOne();

    std::vector<std::function<ObjectPtr()>> spawn_types_;    //!< 生成する投げ物種類の作成関数

    float  wander_radius_   = 25.0f;    //!< 移動範囲の半径
    float  wander_height_   = 25.0f;    //!< 飛行の高さ
    float  move_speed_      = 8.0f;     //!< 移動速度
    float  spawn_time_min_  = 3.0f;     //!< スポーン間隔の最小(秒)
    float  spawn_time_max_  = 8.0f;     //!< スポーン間隔の最大(秒)
    float  spawn_offset_    = 5.0f;     //!< スポーン位置のずれの大きさ

    float3 target_     = {0.0f, 0.0f, 0.0f};    //!< 現在の移動先
    float  spawn_left_ = 0.0f;                  //!< 次のスポーンまでの残り時間(秒)
};
}    // namespace PoyPoy
