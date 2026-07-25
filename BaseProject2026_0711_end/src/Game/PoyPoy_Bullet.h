//---------------------------------------------------------------------------
//!	@file	PoyPoy_Bullet.h
//! @brief	PoyPoy_Bullet
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>

namespace PoyPoy {
USING_PTR(Bullet);
class Bullet : public Object
{
public:
    BP_OBJECT_DECL(Bullet, "PoyPoy::Bullet");

    //! @brief 初期化
    //! @return 初期化終了
    bool Init() override;

    void Update() override;

    void SetDirection(float3 dir);

    void ResetDirection();

    // 当たり判定が行われたときに呼ばれる関数
    void OnHit(const ComponentCollision::HitInfo& hit_info) override;

private:
    float3 direction_ = {0, 1, 0};
    float  speed_     = 2.0f;
    float3 rotation_  = {0, 1, 0};
};
}    // namespace PoyPoy
