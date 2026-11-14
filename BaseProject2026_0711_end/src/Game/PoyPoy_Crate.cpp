//---------------------------------------------------------------------------
//!	@file	PoyPoy_Crate.cpp
//! @brief	PoyPoy_Crate
//---------------------------------------------------------------------------

#include "PoyPoy_Crate.h"
#include "Component/ComponentThrowable.h"
#include "PoyPoy_Crystal.h"
#include <System/Component/ComponentCollisionSphere.h>
#include <System/Component/ComponentEffect.h>
#include <System/Component/ComponentModel.h>
#include <System/Scene.h>

namespace PoyPoy {
//! @brief 初期化
//! @return 初期化終了
bool Crate::Init()
{
    // 壊れやすいそこそこの重さの箱。当たったら壊れるのでバウンドしない
    // 見た目は箱型のCube.xの暫定版(専用の木箱モデルが用意できたら差し替える)
    SetModelPath("data/System/Cube.x");
    SetModelScale(radius_);
    SetThrowSpeed(35.0f);
    SetWeight(20.0f);
    SetBounce(0.0f);
    SetFriction(20.0f);
    SetGravityAccel(98.0f);
    SetSpin(0.0f);        // 転がさない
    SetDamage(0.0f);      // 箱なので直撃しても攻撃力なし(壊れた時だけ中身が落ちる)
    SetLiftTime(3.0f);

    Super::Init();

    SetName("Crate");

    if(!GetModelPath().empty())
        AddComponent<ComponentModel>(GetModelPath())->SetScaleAxisXYZ(GetModelScale());

    // スフィアコリジョン(岩石と同じタイプ)
    AddComponent<ComponentCollisionSphere>()
        ->SetRadius(radius_)
        ->SetMass(GetWeight())
        ->SetCollisionGroup(ComponentCollision::CollisionGroup::THROWABLE)
        ->SetHitCollisionGroup((u32)ComponentCollision::CollisionGroup::GROUND | (u32)ComponentCollision::CollisionGroup::WALL |
                               (u32)ComponentCollision::CollisionGroup::PLAYER |
                               (u32)ComponentCollision::CollisionGroup::ENEMY |
                               (u32)ComponentCollision::CollisionGroup::THROWABLE |
                               // ETC(爆風)にも当たりを設定し、ほかの投げ物と同じ扱いにしておく
                               (u32)ComponentCollision::CollisionGroup::ETC);

    AddComponent<ComponentThrowable>();

    // 投げた(攻撃判定ONの)ものが実ヒットしたら壊れて中身を出す(設置状態は壊れない。爆弾と同じ仕組み)
    if(auto throwable = GetComponent<ComponentThrowable>()) {
        std::weak_ptr<Crate> weak = std::dynamic_pointer_cast<Crate>(SharedThis());
        throwable->SetContactFunc([weak]() {
            if(auto self = weak.lock(); self && !self->broken_) {
                auto th = self->GetComponent<ComponentThrowable>();
                // 投げて攻撃判定がONのものだけが着弾で壊れる(設置状態は壊れない)
                if(th && th->IsArmed())
                    self->Break();
            }
        });
    }

    return true;
}

//! @brief 破壊処理(中身アイテムを出現させエフェクトを再生して消える)
void Crate::Break()
{
    // 当たり多重で複数回呼ばれても一回だけ壊れる
    if(broken_)
        return;
    broken_  = true;
    float3 pos = GetWorldMatrix().translate();

    // 中身アイテム出現(箱の真上に出して重力で落ちつかせるので拾える)
    for(int i = 0; i < item_count_; i++)
        Scene::Object::Create<Crystal>()->SetTranslate(pos + float3(0.0f, 10.0f, 0.0f));

    // 破壊エフェクト(再生終了で自動消滅)
    ComponentEffect::Object::Create("data/Game/Models/Effect/hanabi.efkefc", pos, float3(0, 0, 0), float3(1.0f, 1.0f, 1.0f));

    // 壊れた木箱は消える
    Scene::Object::Release(SharedThis());
}

}    // namespace PoyPoy
