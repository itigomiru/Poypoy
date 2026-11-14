//---------------------------------------------------------------------------
//!	@file	PoyPoy_Bomb.h
//! @brief	PoyPoy_Bomb
//---------------------------------------------------------------------------
#pragma once
#include <Game/PoyPoy_ThrowableObject.h>

namespace PoyPoy {
USING_PTR(Bomb);
USING_PTR(SmallBomb);
class Bomb : public ThrowableObject
{
public:
    BP_OBJECT_DECL(Bomb, "PoyPoy::Bomb");

    //! @brief 初期化
    //! @return 初期化終了
    bool Init() override;

    //! @brief 更新(誘爆予約の残りフレームを進める)
    void Update() override;

    //! @brief 誘爆の予約(爆風に当たったら即爆発せず、少し待ってから爆発する)
    //! @param chain_victims この爆発連鎖で当たり済みの対象(元爆風から受け継いだ shared)
    void ChainDetonate(const std::shared_ptr<std::vector<void*>>& chain_victims);

    //! @brief 着弾で爆発する(直撃ノックバックは通常、直撃ダメージのみ爆風に統合)
    bool ExplodesOnHit() const override { return true; }

    //! @brief ノックバック倍率(爆風で遠くへ吹き飛ぶので2倍)
    float GetKnockbackScale() const override { return 2.0f; }

    //! @brief 爆風の半径の設定
    //! @param radius 半径
    void SetBlastRadius(float radius) { blast_radius_ = radius; }
    //! @brief 爆風の半径の取得
    //! @return 半径
    float GetBlastRadius() const { return blast_radius_; }

    //! @brief 爆発処理(一瞬大きい球コリジョンの爆風を発生させて消える。誘爆と多重ヒットで複数回呼ばれても一回だけ爆発)
    //! @param chain_victims この爆発連鎖で当たり済みの対象(shared。誘爆時は元爆風から受け継ぎ ダメージの二重掛けを防ぐ)
    void Explode(const std::shared_ptr<std::vector<void*>>& chain_victims = nullptr);

protected:
    //! @brief 設計値の設定(派生クラスでオーバーライドして数値を変える)
    virtual void InitSettings();

    //! @brief オブジェクト名(派生クラスでオーバーライド)
    virtual const char* BombName() const { return "Bomb"; }

    float blast_radius_ = 45.0f;    //!< 爆風が当たる半径

private:
    bool exploded_ = false;    //!< 爆発済みか

    int chain_frames_       = -1;    //!< 誘爆予約の残りフレーム(-1=予約なし)
    int chain_delay_frames_ = 10;    //!< 誘爆の遅延フレーム数(爆風に当たってから爆発までの待ち)

    std::shared_ptr<std::vector<void*>> pending_victims_;    //!< 誘爆予約時に元爆風から受け継いだ当たり済みリスト
};

//! @brief 小さい爆弾(標準のボムより爆風が小さく軽い。ノックバック倍率は同じ)
class SmallBomb : public Bomb
{
public:
    BP_OBJECT_DECL(SmallBomb, "PoyPoy::SmallBomb");

protected:
    //! @brief 設計値の設定(標準のボムから小さい分だけ上書き)
    void InitSettings() override;

    //! @brief オブジェクト名
    const char* BombName() const override { return "SmallBomb"; }
};

//! @brief 爆風(爆発地点に一瞬だけ出る大きい球コリジョン。当たったキャラへダメージとノックバック)
class Blast : public Object
{
public:
    BP_OBJECT_DECL(Blast, "PoyPoy::Blast");

    //! @brief 爆風を発生させる(指定位置に指定半径の球コリジョンを作り、数フレームで消える)
    //! @param pos             爆心位置
    //! @param radius          爆風の半径(球コリジョンの半径)
    //! @param damage          ダメージ(受け側がしゃがみ中は倍率軽減)
    //! @param thrower         投げ主(爆風の影響を受けない)
    //! @param knockback_scale ノックバック倍率(爆弾は2倍)
    //! @param chain_victims   この爆発連鎖で当たり済みの対象(shared。誘爆の爆風は元爆風から受け継ぐ)
    static void Spawn(const float3& pos, float radius, float damage, const std::weak_ptr<Object>& thrower, float knockback_scale = 1.0f,
                      const std::shared_ptr<std::vector<void*>>& chain_victims = nullptr);

    //! @brief 初期化(大きい球コリジョンを生成する)
    //! @return 初期化終了
    bool Init() override;

    //! @brief 更新(短時間後に消える)
    void Update() override;

    //! @brief 描画(爆風球を一瞬表示する)
    void Draw() override;

    //! @brief 当たり処理(当たったCharacterへダメージとノックバックを適用、当たったBombは誘爆)
    //! @param hitInfo 当たった情報
    void OnHit(const ComponentCollision::HitInfo& hitInfo) override;

private:
    float                 radius_          = 30.0f;    //!< 爆風の半径(Spawnで設定)
    float                 damage_          = 0.0f;     //!< 爆風ダメージ(Spawnで設定)
    float                 knockback_scale_ = 1.0f;     //!< ノックバック倍率(Spawnで設定、爆弾は2倍)
    std::weak_ptr<Object> thrower_;                    //!< 投げ主(爆風除外用、Spawnで設定)
    int                   life_            = 6;                //!< 残存活命フレーム数
    std::shared_ptr<std::vector<void*>> victims_ = std::make_shared<std::vector<void*>>();    //!< 処理済みの対象ID(対象ごとに一回だけ適用。誘爆した爆風とは同一のリストを共有し二重ダメージを防ぐ)
};
}    // namespace PoyPoy
