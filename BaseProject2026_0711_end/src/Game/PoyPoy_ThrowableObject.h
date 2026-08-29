//---------------------------------------------------------------------------
//!	@file	PoyPoy_ThrowableObject.h
//! @brief	PoyPoy_ThrowableObject
//---------------------------------------------------------------------------
#include <System/Scene.h>

namespace PoyPoy {
USING_PTR(ThrowableObject);
class ThrowableObject : public Object
{
public:
    BP_OBJECT_DECL(ThrowableObject, "PoyPoy::ThrowableObject");

    //! @brief 初期化
    //! @return 初期化終了
    bool Init() override;

    void Update() override;

    // 当たり判定が行われたときに呼ばれる関数
    void OnHit(const ComponentCollision::HitInfo& hit_info) override;

private:
};
}    // namespace PoyPoy
