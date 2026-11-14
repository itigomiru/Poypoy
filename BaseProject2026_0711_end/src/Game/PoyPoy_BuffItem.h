//---------------------------------------------------------------------------
//!	@file	PoyPoy_BuffItem.h
//! @brief	PoyPoy_BuffItem
//---------------------------------------------------------------------------
#pragma once
#include <Game/PoyPoy_ThrowableObject.h>
#include <Game/PoyPoy_Character.h>

namespace PoyPoy {
USING_PTR(BuffItem);

//! @brief バフ・デバフアイテムの基底クラス(投げ物型)。
//!        プレイヤー(Player)へ実ヒット(オーバーラップ除く)すると設計値どおりの効果をプレイヤーへ適用し、
//!        アイテムは見た目だけの「効果運搬体」になって、持続時間ぶん経過すると能力値を元へ戻して消える。
//!        具体的なアイテムは派生クラスを作り InitSettings()(設計値)や EffectName() をオーバーライドする。
//!        效果を自作したい場合は ApplyTo() をオーバーライドする。
class BuffItem : public ThrowableObject
{
public:
    BP_OBJECT_DECL(BuffItem, "PoyPoy::BuffItem");

    //! @brief 効果を適用する能力値の種類
    enum class BuffStat { HP, POWER, MOVE_SPEED };

    //! @brief 初期化
    //! @return 初期化終了
    bool Init() override;

    //! @brief 更新(効果発動後は持続時間を管理し、時間切れで能力値を戻して消える)
    void Update() override;

    //! @brief 効果名(表示用。派生クラスでオーバーライドする)
    virtual const char* EffectName() const { return "BuffItem"; }

    //! @brief デバフか
    //! @return デバフならtrue
    bool IsDebuff() const { return is_debuff_; }

    //! @brief 効果が発動しているか(発動中は見えない効果運搬体になっている)
    bool IsApplied() const { return applied_; }

protected:
    //! @brief 当たり処理(プレイヤーへ当たったら効果を適用してアイテムを消費する)
    void OnHit(const ComponentCollision::HitInfo& hitInfo) override;

    //! @brief 効果の適用(既定実装は能力値を amount_ で書き換え元値を控える。派生クラスでオーバーライド可)
    //! @param character 効果を適用するキャラクター
    //! @return 適用できたならtrue
    virtual bool ApplyTo(const CharacterPtr& character);

    //! @brief 設計値の設定(派生クラスで Super::InitSettings() 呼出し後に上書きする)
    virtual void InitSettings();

    //! @brief 効果の持続を開始する(カスタム ApplyTo 専用。対象と残り時間を控える)
    void StartDuration(const CharacterPtr& character);

    //! @brief 効果終了時に適用した効果を元へ戻す(既定は stat_ の能力値復元。カスタム効果はオーバーライドして戻す)
    virtual void Restore(const CharacterPtr& target);

    float    duration_  = 10.0f;                //!< 効果の持続時間(秒。0=一回限りの効果で、元へは戻さない)
    float    amount_    = 1.5f;                 //!< 効果量(is_add_ が false なら倍率=バフ1.5/デバフ0.5、true なら加算量±amount_)
    bool     is_add_    = false;                //!< amount_ を加算で適用するか
    bool     is_debuff_ = false;                //!< デバフか(加算時の符号と表示用)
    BuffStat stat_      = BuffStat::MOVE_SPEED;    //!< 変更する能力値

private:
    //! @brief 現在の値を設定値で書き換える(Modify)
    float Modify(float value) const;

    //! @brief 効果発動時にアイテムを invisible な運搬体にする(モデル除去・コリジョンと投げ物挙動を無効化)
    void HideAsCarrier();

    bool                   applied_       = false;    //!< 効果発動済みか
    float                  buff_end_time_ = 0.0f;     //!< 効果の消滅時刻(シーン時間。秒)
    std::weak_ptr<Character> target_;                 //!< 能力値を変更した相手(時間切れで戻す)
    float                  restore_value_ = 0.0f;     //!< 時間切れ時に戻す能力値
};
}    // namespace PoyPoy
