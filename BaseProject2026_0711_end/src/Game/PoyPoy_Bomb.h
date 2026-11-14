//---------------------------------------------------------------------------
//!	@file	PoyPoy_Bomb.h
//! @brief	PoyPoy_Bomb
//---------------------------------------------------------------------------
#pragma once
#include <Game/PoyPoy_ThrowableObject.h>

namespace PoyPoy {
USING_PTR(Bomb);
class Bomb : public ThrowableObject
{
public:
    BP_OBJECT_DECL(Bomb, "PoyPoy::Bomb");

    //! @brief 初期化
    //! @return 初期化終了
    bool Init() override;

    //! @brief 着弾で爆発する(直撃ノックバックは通常、直撃ダメージのみ爆風に統合)
    bool ExplodesOnHit() const override { return true; }

    //! @brief 爆風の半径の設定
    //! @param radius 半径
    void SetBlastRadius(float radius) { blast_radius_ = radius; }
    //! @brief 爆風の半径の取得
    //! @return 半径
    float GetBlastRadius() const { return blast_radius_; }

private:
    float blast_radius_ = 45.0f;    //!< 爆風が当たる半径
    bool  exploded_     = false;    //!< 爆発済みか

    //! @brief 爆発処理(一瞬大きい球コリジョンの爆風を発生させて消える)
    void Explode();
};

//! @brief 爆風(爆発地点に一瞬だけ出る大きい球コリジョン。当たったキャラへダメージとノックバック)
class Blast : public Object
{
public:
    BP_OBJECT_DECL(Blast, "PoyPoy::Blast");

    //! @brief 爆風を発生させる(指定位置に指定半径の球コリジョンを作り、数フレームで消える)
    //! @param pos     爆心位置
    //! @param radius  爆風の半径(球コリジョンの半径)
    //! @param damage  ダメージ(受け側がしゃがみ中は倍率軽減)
    //! @param thrower 投げ主(爆風の影響を受けない)
    static void Spawn(const float3& pos, float radius, float damage, const std::weak_ptr<Object>& thrower);

    //! @brief 初期化(大きい球コリジョンを生成する)
    //! @return 初期化終了
    bool Init() override;

    //! @brief 更新(短時間後に消える)
    void Update() override;

    //! @brief 描画(爆風球を一瞬表示する)
    void Draw() override;

    //! @brief 当たり処理(当たったCharacterへダメージとノックバックを適用)
    //! @param hitInfo 当たった情報
    void OnHit(const ComponentCollision::HitInfo& hitInfo) override;

private:
    float                 radius_ = 30.0f;    //!< 爆風の半径(Spawnで設定)
    float                 damage_ = 0.0f;     //!< 爆風ダメージ(Spawnで設定)
    std::weak_ptr<Object> thrower_;           //!< 投げ主(爆風除外用、Spawnで設定)
    int                   life_ = 6;          //!< 残存活命フレーム数
    std::vector<void*>    victims_;           //!< 処理済みの対象ID(対象ごとに一回だけ適用)
};
}    // namespace PoyPoy
