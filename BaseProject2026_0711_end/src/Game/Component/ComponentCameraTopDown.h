#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>

USING_PTR(ComponentCameraTopDown);

class ComponentCameraTopDown : public Component
{
public:
    BP_COMPONENT_DECL(ComponentCameraTopDown, u8"トップダウンカメラ");

    void Init() override;

    void Update() override;

    //! @brief 見下ろしの固定中心(注視点)の設定
    ComponentCameraTopDownPtr SetFocusCenter(float3 center);

    //! @brief 視線を寄せる対象の設定(将来のキャラクター追従用)
    ComponentCameraTopDownPtr SetFollowTarget(ObjectPtr target);

    //! @brief 視線を寄せる対象を名前で設定(名前は毎フレーム解決する)
    ComponentCameraTopDownPtr SetFollowName(std::string_view name);

    //! @brief 注視点を中心から対象の位置へ寄せる割合(0..1)の設定
    ComponentCameraTopDownPtr SetFollowWeight(float weight);

    //! @brief 焦点からのカメラ高さの設定
    ComponentCameraTopDownPtr SetHeight(float height);

    //! @brief 水平からの見下ろし角度の設定[度](20..88、88でほぼ真上)
    ComponentCameraTopDownPtr SetPitch(float pitch);

    //! @brief 画面の上になる世界方向の設定[度](0=世界+Z、+方向で+Xへ回旋)
    ComponentCameraTopDownPtr SetScreenYaw(float yaw);

    //! @brief 注視点の追従速度の設定(大きいほど速く追う)
    ComponentCameraTopDownPtr SetSmooth(float speed);

    void GUI() override;

private:
    float3 focus_center_ = {0.0f, 0.0f, 0.0f};    //!< 見下ろす固定中心
    float3 focus_        = {0.0f, 0.0f, 0.0f};    //!< 毎フレーム適用する現在の焦点

    ObjectWeakPtr follow_;          //!< 視線を寄せる対象(将来用の下受け)
    std::string follow_name_;       //!< 視線を寄せる対象の名前
    float follow_weight_ = 0.0f;    //!< 追従割合(0..1) 0=完全固定

    float height_     = 140.0f;    //!< 焦点からのカメラ高さ
    float pitch_      = 40.0f;    //!< 水平からの見下ろし角度[度]
    float screen_yaw_ = 0.0f;     //!< 画面上が向く世界方向[度]
    float smooth_     = 6.0f;     //!< 注視点の追従速度

    //--------------------------------------------------------------------
    //! @name Cereal処理
    //--------------------------------------------------------------------
    //@{

    //! @brief セーブロード
    //! @param arc アーカイバ
    //! @param ver バージョン
    CEREAL_SAVELOAD(arc, ver)
    {
        arc(CEREAL_NVP(focus_center_), CEREAL_NVP(focus_));
        arc(CEREAL_NVP(follow_));
        arc(CEREAL_NVP(follow_name_));
        arc(CEREAL_NVP(follow_weight_));
        arc(CEREAL_NVP(height_));
        arc(CEREAL_NVP(pitch_));
        arc(CEREAL_NVP(screen_yaw_));
        arc(CEREAL_NVP(smooth_));

        arc(cereal::make_nvp("Component", cereal::base_class<Component>(this)));

        Init();
    }

    //@}
};

CEREAL_CLASS_VERSION(ComponentCameraTopDown, 1);
