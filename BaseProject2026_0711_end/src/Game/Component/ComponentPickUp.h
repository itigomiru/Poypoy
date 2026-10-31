#pragma once
#include "ComponentThrowable.h"
#include <System/Scene.h>
#include <System/Component/Component.h>

USING_PTR(ComponentPickUp);

class ComponentPickUp : public Component
{
public:
    BP_COMPONENT_DECL(ComponentPickUp, u8"持ち上げる");
    void Update() override;
    void LateUpdate() override;

    void GUI() override;

    void SetThrowableObj(ComponentThrowablePtr throwable_obj) { throwable_obj_ = throwable_obj; }

    void SetCanPickUp(bool can_pickUp) { can_pickUp_ = can_pickUp; }
    bool GetCanPickUp() const { return can_pickUp_; }

    enum class PickUpState {
        None,
        PickingUp,   // アニメーション再生中（もつ）
        Holding,     // 持ち上げている
        Throwing,    // アニメーション再生中（投げる）
    };

    PickUpState GetState() const { return state_; }

private:
    bool can_pickUp_ = false;
    PickUpState state_ = PickUpState::None;
    ComponentThrowablePtr throwable_obj_ = nullptr;

    void ExecutePickUp();
    void ExecuteThrow();

    //--------------------------------------------------------------------
    //! @name Cereal処理
    //--------------------------------------------------------------------
    //@{
    //! @brief セーブ
    // @param arc アーカイバ
    // @param ver バージョン
    CEREAL_SAVELOAD(arc, ver) { arc(cereal::make_nvp("Component", cereal::base_class<Component>(this))); }
};
