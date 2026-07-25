//---------------------------------------------------------------------------
//!	@file	PoyPoy_Camera.h
//! @brief	PoyPoy_Camera
//---------------------------------------------------------------------------
#include <System/Scene.h>

namespace PoyPoy {
USING_PTR(Camera);
class Camera : public Object
{
public:
    BP_OBJECT_DECL(Camera, "PoyPoy::Camera");

    //! @brief 初期化
    //! @return 初期化終了
    bool Init() override;
};
}    // namespace PoyPoy
