//---------------------------------------------------------------------------
//!	@file	PoyPoy_Dummy.h
//! @brief	PoyPoy_Dummy
//---------------------------------------------------------------------------
#include <System/Scene.h>

namespace PoyPoy {
USING_PTR(Dummy);
class Dummy : public Object
{
public:
    BP_OBJECT_DECL(Dummy, "PoyPoy::Dummy");

    //! @brief 初期化
    //! @return 初期化終了
    bool Init() override;

    void Update() override;

    void OnHit(const ComponentCollision::HitInfo& hitInfo) override;
};
}    // namespace PoyPoy
