//---------------------------------------------------------------------------
//!	@file	PoyPoy_Throwable.h
//! @brief	PoyPoy_Throwable
//---------------------------------------------------------------------------
#pragma once
#include <System/Object.h>
#include <string>

namespace PoyPoy {
USING_PTR(Throwable);

//! @brief 投げ物各種の共通ベースクラス。見た目・形・物理設定をキャラ(Character)と同じ形で持つ。
//!        種類を増やすときは PoyPoy::ThrowableObject を継承し、Init() で Super::Init() を呼ぶ前に
//!        各 Set～ を呼んで設定する(コンポーネント側は Init 時にこの値を受け取る)。
class Throwable : public Object
{
public:
    BP_OBJECT_DECL(Throwable, "PoyPoy::Throwable");

    //! @brief 初期化
    //! @return 初期化終了
    bool Init() override;

    //! @brief モデルパス取得
    inline const std::string& GetModelPath() const { return model_path_; }

    //! @brief モデルパス設定(Init前に呼ぶこと)
    ThrowablePtr SetModelPath(const std::string& path) { model_path_ = path; return std::static_pointer_cast<Throwable>(SharedThis()); }

    //! @brief モデル拡縮取得
    inline float GetModelScale() const { return model_scale_; }

    //! @brief モデル拡縮設定(Init前に呼ぶこと)
    ThrowablePtr SetModelScale(float scale) { model_scale_ = scale; return std::static_pointer_cast<Throwable>(SharedThis()); }

    //! @brief カプセルコリジョンの半径取得
    inline float GetRadius() const { return radius_; }

    //! @brief カプセルコリジョンの半径設定(Init前に呼ぶこと)
    ThrowablePtr SetRadius(float radius) { radius_ = radius; return std::static_pointer_cast<Throwable>(SharedThis()); }

    //! @brief カプセルコリジョンの高さ取得
    inline float GetHeight() const { return height_; }

    //! @brief カプセルコリジョンの高さ設定(Init前に呼ぶこと)
    ThrowablePtr SetHeight(float height) { height_ = height; return std::static_pointer_cast<Throwable>(SharedThis()); }

    //! @brief 投げたときの初速度取得
    inline float GetThrowSpeed() const { return throw_speed_; }

    //! @brief 投げたときの初速度設定(Init前に呼ぶこと)
    ThrowablePtr SetThrowSpeed(float speed) { throw_speed_ = speed; return std::static_pointer_cast<Throwable>(SharedThis()); }

    //! @brief 重さ取得
    inline float GetWeight() const { return weight_; }

    //! @brief 重さ設定(Init前に呼ぶこと)
    ThrowablePtr SetWeight(float weight) { weight_ = weight; return std::static_pointer_cast<Throwable>(SharedThis()); }

    //! @brief バウンドの強さ(跳返り係数)取得
    inline float GetBounce() const { return bounce_; }

    //! @brief バウンドの強さ設定(Init前に呼ぶこと)
    ThrowablePtr SetBounce(float bounce) { bounce_ = bounce; return std::static_pointer_cast<Throwable>(SharedThis()); }

    //! @brief 摩擦の強さ(摩擦係数)取得
    inline float GetFriction() const { return friction_; }

    //! @brief 摩擦の強さ設定(Init前に呼ぶこと)
    ThrowablePtr SetFriction(float friction) { friction_ = friction; return std::static_pointer_cast<Throwable>(SharedThis()); }

    //! @brief 重力加速度取得
    inline float GetGravityAccel() const { return gravity_accel_; }

    //! @brief 重力加速度設定(Init前に呼ぶこと)
    ThrowablePtr SetGravityAccel(float gravity) { gravity_accel_ = gravity; return std::static_pointer_cast<Throwable>(SharedThis()); }

    //! @brief 飛行中の回転速度の大きさ(度/秒)取得
    inline float GetSpin() const { return spin_; }

    //! @brief 飛行中の回転速度の大きさ設定(Init前に呼ぶこと 0で回転なし)
    ThrowablePtr SetSpin(float spin) { spin_ = spin; return std::static_pointer_cast<Throwable>(SharedThis()); }

    //! @brief 投げ命中時のダメージ取得(0でダメージなし)
    inline float GetDamage() const { return damage_; }

    //! @brief 投げ命中時のダメージ設定(Init前に呼ぶこと)
    ThrowablePtr SetDamage(float damage) { damage_ = damage; return std::static_pointer_cast<Throwable>(SharedThis()); }

    //! @brief 重さによる保持時間の上限取得(秒 -1で無制限)
    inline float GetLiftTime() const { return lift_time_; }

    //! @brief 重さによる保持時間の上限設定(Init前に呼ぶこと 反映は ComponentPickUp 側の将来課題)
    ThrowablePtr SetLiftTime(float time) { lift_time_ = time; return std::static_pointer_cast<Throwable>(SharedThis()); }

protected:
    std::string model_path_  = "data/Game/Models/Object/Crystal.mv1";   //!< 使うモデルのパス
    float       model_scale_ = 0.2f;                                     //!< モデル拡縮
    float       radius_      = 5.0f;                                     //!< カプセルコリジョンの半径
    float       height_      = 38.0f;                                    //!< カプセルコリジョンの高さ
    float       throw_speed_ = 50.0f;                                    //!< 投げたときの初速度
    float       weight_      = 20.0f;                                    //!< 重さ(コリジョン質量)
    float       bounce_      = 0.25f;                                    //!< バウンドの強さ(跳返り係数 0.0f～1.0f)
    float       friction_    = 15.0f;                                    //!< 摩擦の強さ(摩擦係数 0.0f～)
    float       gravity_accel_ = 98.0f;                                  //!< 重力加速度
    float       spin_        = 180.0f;                                   //!< 飛行中のY軸回り回転速度の大きさ(度/秒 0でなし)
    float       damage_      = 0.0f;                                     //!< 投げ命中時のダメージ(0でなし)
    float       lift_time_   = -1.0f;                                    //!< 重さによる保持時間の上限(秒 -1で無制限)
};
}    // namespace PoyPoy
