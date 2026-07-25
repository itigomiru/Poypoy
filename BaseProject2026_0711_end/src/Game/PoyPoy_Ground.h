//---------------------------------------------------------------------------
//!	@file	PoyPoy_Ground.h
//! @brief	PoyPoy_Ground
//---------------------------------------------------------------------------
#include <System/Scene.h>

namespace PoyPoy {
USING_PTR(Ground);
class Ground : public Object
{
public:
    BP_OBJECT_DECL(Ground, "PoyPoy::Ground");

    //! @brief 初期化
    //! @return 初期化終了
    bool Init() override;
};
}    // namespace PoyPoy
