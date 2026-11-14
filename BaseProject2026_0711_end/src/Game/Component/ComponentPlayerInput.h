//---------------------------------------------------------------------------
//!	@file	ComponentPlayerInput.h
//! @brief	プレイヤー入力
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>
#include <Game/Component/ComponentInput.h>

USING_PTR(ComponentPlayerInput);

//===========================================================================
//! @brief プレイヤー入力
//! @details 物理入力(Input::)をそのまま読み出す入力ソースです。
//!          キーボード・パッドの入力がそのままキャラの操作になります。
//===========================================================================
class ComponentPlayerInput : public ComponentInput
{
public:
    BP_COMPONENT_DECL(ComponentPlayerInput, u8"プレイヤー入力");

    void Init() override;

    //--------------------------------------------------------------------
    //! @name 入力検証
    //! @details 物理入力をそのまま読み出します
    //@{

    //! @brief 指定キーの押下検証
    bool IsKey(int keyID) const override;

    //! @brief 指定キーの1回だけ押下検証
    bool IsKeyDown(int keyID) const override;

    //! @brief 指定キーが離された検証
    bool IsKeyUp(int keyID) const override;

    //! @brief 指定キーの初回(1フレーム目)押下検証
    bool IsKeyOn(int keyID) const override;

    //! @brief 指定キーが押されていない検証
    bool IsKeyRelease(int keyID) const override;

    //! @brief 指定キーの長押し検証(指定フレーム以上押されている)
    bool IsKeyRepeat(int keyID, u32 frame = 2) const override;

    //@}

private:
    //--------------------------------------------------------------------
    //! @name Cereal処理
    //--------------------------------------------------------------------
    //@{

    //! @brief セーブ
    // @param arc アーカイバ
    // @param ver バージョン
    CEREAL_SAVELOAD(arc, ver)
    {
        // 入力の基底もたどってアクションキーを保存する
        arc(cereal::make_nvp("Component", cereal::base_class<ComponentInput>(this)));
    }
};

CEREAL_CLASS_VERSION(ComponentPlayerInput, 1);
