#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>

USING_PTR(ComponentPickUp);

class ComponentPickUp : public Component
{
public:
    BP_COMPONENT_DECL(ComponentPickUp, u8"持ち上げる");

    void GUI() override;

    void SetCanPickUp(bool can_pickUp) { can_pickUp_ = can_pickUp; }
    bool GetCanPickUp() const { return can_pickUp_; }
    void SetIsPickup(bool is_pickup) { is_pickup_ = is_pickup; }
    bool GetIsPickup() const { return is_pickup_; }

private:
    //--------------------------------------------------------------------
    //! @name Cereal処理
    //--------------------------------------------------------------------
    //@{
    
    bool can_pickUp_ = false;
    bool is_pickup_ = false;
    //! @brief セーブ
    // @param arc アーカイバ
    // @param ver バージョン
    CEREAL_SAVELOAD(arc, ver) { arc(cereal::make_nvp("Component", cereal::base_class<Component>(this))); }
};
