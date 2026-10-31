//---------------------------------------------------------------------------
//!	@file	PoyPoy_Dummy.h
//! @brief	PoyPoy_Dummy
//---------------------------------------------------------------------------
#include <System/Scene.h>
#include <System/Component/ComponentModel.h>

namespace PoyPoy {
USING_PTR(Dummy);
class Dummy : public Object
{
public:
    BP_OBJECT_DECL(Dummy, "PoyPoy::Dummy");

    //! @brief 初期化
    //! @return 初期化終了
    bool Init() override;
};
}    // namespace PoyPoy
