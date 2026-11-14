//---------------------------------------------------------------------------
//!	@file	PoyPoy_Crate.h
//! @brief	PoyPoy_Crate
//---------------------------------------------------------------------------
#pragma once
#include <Game/PoyPoy_Throwable.h>

namespace PoyPoy {
USING_PTR(Crate);

//! @brief 木箱(中身入りの投げ物)。
//!        投げて何かに実ヒットすると壊れて、中身アイテム(Crystal)が出てくる。
//!        設置状態(未投下)では壊れない。ほかの投げ物と同様 Throwable を直接継承してスフィアコリジョンを自前で持つ。
class Crate : public Throwable
{
public:
    BP_OBJECT_DECL(Crate, "PoyPoy::Crate");

    //! @brief 初期化(スフィアコリジョンとComponentThrowableを自前で生成)
    //! @return 初期化終了
    bool Init() override;

    //! @brief 壊れた時に出てくるアイテム数の設定
    //! @param count アイテム数
    void SetItemCount(int count) { item_count_ = count; }

    //! @brief 壊れたか
    //! @return 壊れていればtrue
    bool IsBroken() const { return broken_; }

protected:
    //! @brief 破壊処理(中身アイテムを出現させエフェクトを再生して消える)
    void Break();

    float radius_     = 4.0f;    //!< スフィアコリジョンの半径(Cube.xモデルの拡縮にも使う)
    int   item_count_ = 1;       //!< 壊れた時に出てくるアイテム数
    bool  broken_     = false;   //!< 破壊済みか
};
}    // namespace PoyPoy
