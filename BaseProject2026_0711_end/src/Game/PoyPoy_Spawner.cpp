//---------------------------------------------------------------------------
//!	@file	PoyPoy_Spawner.cpp
//! @brief	PoyPoy_Spawner
//---------------------------------------------------------------------------

#include "PoyPoy_Spawner.h"
#include "PoyPoy_Crystal.h"
#include "PoyPoy_SmallCrystal.h"
#include <System/VectorMath.h>

namespace PoyPoy {
//! @brief 初期化
//! @return 初期化終了
bool Spawner::Init()
{
    Super::Init();

    SetName("Spawner");

    // スポーン源だと分かるように水晶モデルを大きく出して目に見えるようにする
    AddComponent<ComponentModel>("data/Game/Models/Object/Crystal.mv1")->SetScaleAxisXYZ(1.0f);

    // 生成する種類(Crystal と SmallCrystal をランダムに選ぶ)
    spawn_types_.push_back([]() -> ObjectPtr { return Scene::Object::Create<Crystal>(); });
    spawn_types_.push_back([]() -> ObjectPtr { return Scene::Object::Create<SmallCrystal>(); });

    target_     = RandomTarget();
    spawn_left_ = RandomInterval();

    return true;
}

//! @brief 更新(移動とスポーン実行)
void Spawner::Update()
{
    Super::Update();

    float dt = GetDeltaTime();

    // 移動先へ移動し続ける(到着したら新しいランダム地点を作る)
    float3 pos = GetWorldMatrix().translate();
    float3 dir = target_ - pos;

    if((float)length(dir) <= 1.0f) {
        target_ = RandomTarget();
    }
    else {
        float step = move_speed_ * dt;
        pos        = pos + normalize(dir) * (step < (float)length(dir) ? step : (float)length(dir));
        SetTranslate(pos);
    }

    // 時間になったら周囲に投げ物を1つスポーンさせる
    spawn_left_ -= dt;
    if(spawn_left_ <= 0.0f) {
        SpawnOne();
        spawn_left_ = RandomInterval();
    }
}

//! @brief 移動先のランダム地点を作る
float3 Spawner::RandomTarget()
{
    static std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution<float> dist(-wander_radius_, wander_radius_);

    return float3(dist(rng), wander_height_, dist(rng));
}

//! @brief スポーン間隔のランダム値を作る
float Spawner::RandomInterval()
{
    static std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution<float> dist(spawn_time_min_, spawn_time_max_);

    return dist(rng);
}

//! @brief 投げ物を1つランダムな種類で生成する
void Spawner::SpawnOne()
{
    static std::mt19937 rng{std::random_device{}()};

    if(spawn_types_.empty())
        return;

    std::uniform_real_distribution<float> dist(-spawn_offset_, spawn_offset_);
    std::uniform_int_distribution<int> type_dist(0, static_cast<int>(spawn_types_.size()) - 1);

    auto obj = spawn_types_[type_dist(rng)]();

    // 自分の周囲のずれぶんだけずらして出現させる(そのまま落下していく)
    float3 pos = GetWorldMatrix().translate() + float3(dist(rng), 0.0f, dist(rng));
    obj->SetTranslate(pos);
}

}    // namespace PoyPoy
