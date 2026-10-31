#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>
#include <System/Component/ComponentCollision.h>

USING_PTR(ComponentThrowable);

class ComponentThrowable : public Component
{
public:
    BP_COMPONENT_DECL(ComponentThrowable, u8"投げられるもの");

    void Init() override;

    void Update() override;

    void GUI() override;

    //! @brief オーナーのコリジョンから呼ばれる当たり処理(重力・バウンド・摩擦をここで扱う)
    //! @param hitInfo 当たった情報
    void OnContact(const ComponentCollision::HitInfo& hitInfo);

    //! @brief 持ち上げ状態にする(重力・移動を止める)
    void PickUp();

    //! @brief 投げる。指定方向に初速度を設定する
    //! @param dir 投げる方向
    void Throw(const float3& dir);

    //! @brief 投げたときの初速度の設定
    //! @param speed 初速度
    void SetThrowSpeed(float speed) { throw_speed_ = speed; }
    //! @brief 投げたときの初速度の取得
    //! @return 初速度
    float GetThrowSpeed() const { return throw_speed_; }

    //! @brief 重さ(コリジョンの押し戻されにくさ)の設定
    //! @param weight 重さ(0.0f超)
    void SetWeight(float weight) { weight_ = weight; ApplySettings(); }
    //! @brief 重さの取得
    //! @return 重さ
    float GetWeight() const { return weight_; }

    //! @brief バウンドの強さ(跳返り係数)の設定
    //! @param bounce 跳返り係数(0.0f～1.0f)
    void SetBounce(float bounce) { bounce_ = bounce; }
    //! @brief バウンドの強さ(跳返り係数)の取得
    //! @return 跳返り係数
    float GetBounce() const { return bounce_; }

    //! @brief 摩擦の強さ(摩擦係数)の設定
    //! @param friction 摩擦係数(0.0f～)
    void SetFriction(float friction) { friction_ = friction; }
    //! @brief 摩擦の強さ(摩擦係数)の取得
    //! @return 摩擦係数
    float GetFriction() const { return friction_; }

    //! @brief 重力加速度の設定
    //! @param gravity 重力加速度
    void SetGravityAccel(float gravity) { gravity_accel_ = gravity; }
    //! @brief 重力加速度の取得
    //! @return 重力加速度
    float GetGravityAccel() const { return gravity_accel_; }

    //! @brief 現在の速度の取得
    //! @return 速度
    const float3& GetVelocity() const { return velocity_; }

    //! @brief 最後に接地した地面の高さの推定値の取得
    //! @return 地面の高さ
    float GetGroundHeight() const { return ground_height_; }

private:
    float throw_speed_   = 50.0f;    //!< 投げたときの初速度
    float weight_        = 20.0f;    //!< 重さ(コリジョン質量)
    float bounce_        = 0.25f;     //!< バウンドの強さ(跳返り係数 0.0f～1.0f)
    float friction_      = 15.0f;     //!< 摩擦の強さ(摩擦係数 0.0f～)
    float gravity_accel_ = 98.0f;    //!< 重力加速度

    float spin_ = 180.0f;    //!< 飛行中のY軸回り回転速度(度/秒 0でなし)

    float  spin_rate_   = 0.0f;            //!< 現在の回転角速度(rad/s)
    float  spin_angle_  = 0.0f;            //!< 投げからの累積回転角(rad)
    float3 spin_axis_   = {0.0f, 1.0f, 0.0f};   //!< 投げ時に捕捉した回転軸(ローカルY軸)
    float3 spin_base_x_ = {1.0f, 0.0f, 0.0f};   //!< 投げ時のX軸
    float3 spin_base_z_ = {0.0f, 0.0f, 1.0f};   //!< 投げ時のZ軸

    float3 velocity_ = {0.0f, 0.0f, 0.0f};    //!< 現在の速度
    bool   held_     = false;                 //!< 持ち上げ状態か
    bool   grounded_ = false;                 //!< 前フレームに接地していたか

    float  ground_height_  = 0.0f;                 //!< 接地時の地面の高さ推定値
    float3 contact_normal_ = {0.0f, 0.0f, 0.0f};   //!< デバッグ用:直近の当たり法線
    float  contact_pen_    = 0.0f;                 //!< デバッグ用:直近のめり込み量
    int    dbg_contacts_   = 0;                    //!< デバッグ用:フレーム中の当たり呼び出し回数

    //! @brief 設定した値をオーナーのコリジョンに適用します
    void ApplySettings();

    //--------------------------------------------------------------------
    //! @name Cereal処理
    //--------------------------------------------------------------------
    //@{

    //! @brief セーブ
    // @param arc アーカイバ
    // @param ver バージョン
    CEREAL_SAVELOAD(arc, ver)
    {
        arc(CEREAL_NVP(throw_speed_),
            CEREAL_NVP(weight_),
            CEREAL_NVP(bounce_),
            CEREAL_NVP(friction_));
        if(ver >= 1) {
            arc(CEREAL_NVP(gravity_accel_));
        }
        if(ver >= 2) {
            arc(CEREAL_NVP(spin_));
        }
        arc(cereal::make_nvp("Component", cereal::base_class<Component>(this)));
    }
    //@}
};

CEREAL_CLASS_VERSION(ComponentThrowable, 2);
