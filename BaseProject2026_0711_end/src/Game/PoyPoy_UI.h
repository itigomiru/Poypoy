//---------------------------------------------------------------------------
//!	@file	PoyPoy_UI.h
//! @brief	PoyPoy_UI
//---------------------------------------------------------------------------
#include <System/Scene.h>

namespace PoyPoy {

class PoyPoy_UI : public Scene::Base
{
public:
    BP_CLASS_DECL(PoyPoy_UI, u8"PoyPoyタイトル");
    //! @brief 初期化
    //! @return 初期化済み
    bool Init() override;

    void Update() override;

    void LateDraw() override;

    void GUI() override;

private:
    int UI_x_    = 20;
    int UI_y_    = 110;
    int UI_size_ = 500;
};

}    // namespace PoyPoy
