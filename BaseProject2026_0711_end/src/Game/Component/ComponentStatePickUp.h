#pragma once
#include <System/Scene.h>
#include <Game/Component/ComponentState.h>
#include <Game/PoyPoy_Bullet.h>

USING_PTR(ComponentStatePickUp);

class ComponentStatePickUp : public ComponentState
{
public:
    BP_COMPONENT_DECL(ComponentStatePickUp, u8"持ち上げる");

    void Init() override;

    void Update() override;

    void GUI() override;

private:
    PoyPoy::BulletWeakPtr bullet_;
    bool                  threw_ = false;

    //--------------------------------------------------------------------
    //! @name Cereal処理
    //--------------------------------------------------------------------
    //@{

    //! @brief セーブ
    // @param arc アーカイバ
    // @param ver バージョン
    CEREAL_SAVELOAD(arc, ver) { arc(cereal::make_nvp("Component", cereal::base_class<Component>(this))); }
};

CEREAL_CLASS_VERSION(ComponentStatePickUp, 1);
