//---------------------------------------------------------------------------
//!	@file	PoyPoy_Title.h
//! @brief	PoyPoy_Title
//---------------------------------------------------------------------------
#include <System/Scene.h>

namespace PoyPoy {

class PoyPoy_Title : public Scene::Base
{
public:
    BP_CLASS_DECL(PoyPoy_Title, u8"PoyPoyタイトル");
    //! @brief 初期化
    //! @return 初期化済み
    bool Init() override;

    void Update() override;

    void LateDraw() override;

    void GUI() override;

private:
    int title_x_    = 20;
    int title_y_    = 110;
    int title_size_ = 500;
};

}    // namespace PoyPoy
