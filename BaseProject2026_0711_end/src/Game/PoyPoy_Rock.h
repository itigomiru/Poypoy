//---------------------------------------------------------------------------
//!	@file	PoyPoy_Rock.h
//! @brief	PoyPoy_Rock
//---------------------------------------------------------------------------
#pragma once
#include <Game/PoyPoy_Throwable.h>

namespace PoyPoy {
USING_PTR(Rock);

//! @brief 岩石(コリジョンがスフィアの投げ物)。
//!        ほかの投げ物(ThrowableObject)はカプセルコリジョンだが、岩石は丸いスフィアコリジョンを持つ。
//!        転がらない設定で、重くて積み重ねやすい。見た目は専用モデル未入手のためCube.xの暫定版。
class Rock : public Throwable
{
public:
    BP_OBJECT_DECL(Rock, "PoyPoy::Rock");

    //! @brief 初期化(スフィアコリジョンとComponentThrowableを自前で生成。ThrowableObjectはカプセルを生成するため継承しない)
    //! @return 初期化終了
    bool Init() override;

    //! @brief スフィアコリジョンの半径の取得
    //! @return 半径
    float GetRadius() const { return radius_; }

protected:
    float radius_ = 4.0f;    //!< スフィアコリジョンの半径(Cube.xモデルの拡縮にも使う)
};
}    // namespace PoyPoy
