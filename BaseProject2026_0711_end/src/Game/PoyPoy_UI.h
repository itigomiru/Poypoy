//---------------------------------------------------------------------------
//!	@file	PoyPoy_UI.h
//! @brief	PoyPoy_UI
//---------------------------------------------------------------------------
#include <System/Scene.h>

namespace PoyPoy {

class PoyPoy_UI
{
public:
    BP_CLASS_DECL(PoyPoy_UI, u8"PoyPoyタイトル");
    //! @brief 初期化
    //! @return 初期化済み
    bool Init();

    void Update();

    void Draw();

    void GUI();

private:
    int UI_x_    = 100;
    int UI_y_    = 550;
    int UI_size_ = 100;
    int UI_Box   = -1;

    int Player_num_ = 4;
};

}    // namespace PoyPoy
