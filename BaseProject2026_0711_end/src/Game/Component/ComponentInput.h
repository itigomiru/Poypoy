//---------------------------------------------------------------------------
//!	@file	ComponentInput.h
//! @brief	入力ソース基底
//---------------------------------------------------------------------------
#pragma once
#include <map>
#include <vector>
#include <System/Scene.h>
#include <System/Component/Component.h>

USING_PTR(ComponentInput);

//! @brief 操作タイプ
enum struct ControlType : u8
{
    Player,    //!< プレイヤー操作
    AI,        //!< AI操作
};

//===========================================================================
//! @brief 入力ソース基底
//! @details キャラが読み取る入力の出所を切り替えるための共通インターフェースです。
//!          PlayerInputとAIInputがこの押下検証APIを提供するため、
//!          読む側の処理は操作タイプを意識せずに同じ処理で動きます。
//===========================================================================
class ComponentInput : public Component
{
public:
    BP_COMPONENT_DECL(ComponentInput, u8"入力ベース");

    void Init() override;

    //--------------------------------------------------------------------
    //! @name 入力検証
    //! @details 物理入力(Input::)と同じ検証ができます。
    //!          AIInputはPreUpdateで生成した仮想キーを、
    //!          PlayerInputは物理入力をここから読み出します。
    //@{

    //! @brief 指定キーの押下検証
    virtual bool IsKey(int keyID) const;

    //! @brief 指定キーの1回だけ押下検証
    virtual bool IsKeyDown(int keyID) const;

    //! @brief 指定キーが離された検証
    virtual bool IsKeyUp(int keyID) const;

    //! @brief 指定キーの初回(1フレーム目)押下検証
    virtual bool IsKeyOn(int keyID) const;

    //! @brief 指定キーが押されていない検証
    virtual bool IsKeyRelease(int keyID) const;

    //! @brief 指定キーの長押し検証(指定フレーム以上押されている)
    virtual bool IsKeyRepeat(int keyID, u32 frame = 2) const;

    //@}

    //--------------------------------------------------------------------
    //! @name 操作タイプ
    //@{

    //! @brief 操作タイプを取得します
    ControlType GetControlType() const { return control_type_; }

    //@}

    //--------------------------------------------------------------------
    //! @name アクションキー
    //! @details 攻撃・持ち上げのようにPlayerでもAIでも同じ行動になるキーは、
    //!          ここを正として読みます。読む側でキーを別々に持たれません。
    //@{

    //! @brief アクション(攻撃・持ち上げ)キーを取得します
    int GetActionKey() const { return action_key_; }

    //! @brief アクション(攻撃・持ち上げ)キーをセットします
    void SetActionKey(int key);

    //@}

    //--------------------------------------------------------------------
    //! @name 入力ソースの設定/取得
    //@{

    //! @brief オブジェクトの入力ソースを取得します
    //! @param [in] owner 取得するオブジェクト
    //! @details 入力ソースが未設定のオブジェクトには物理入力を1体ごとに付与します。
    //!          物理入力を全キャラで共有すると、複数のキャラが同じキーボードを読んで
    //!          操作が混ざってしまうためです。nullにはなりません。
    static ComponentInputPtr Get(Object* owner);

    //! @brief 操作タイプに応じた入力ソースを作成しオブジェクトに設定します
    //! @param [in] owner 設定するオブジェクト
    //! @param [in] type  操作タイプ
    //! @details 同じ操作タイプが設定済みなら既存の入力を返します。
    //!          別の操作タイプの場合は設定されている入力ソースを置き換えます。
    static ComponentInputPtr Create(Object* owner, ControlType type);

    //@}

protected:
    //! @brief 仮想キーの入力状態
    struct KeyState
    {
        u32 now_   = 0;    //!< 現在の状態
        u32 old_   = 0;    //!< 1フレーム前の状態
        u32 count_ = 0;    //!< 押されているフレーム数
    };

    //! @brief 押下された仮想キーを受け取り、物理入力と同じ状態遷移で更新します
    //! @param [in] presses このフレーム押下されたキーのリスト
    //! @attention 1フレームに1回だけ呼んでください。
    void UpdateKeys(const std::vector<int>& presses);

    std::map<int, KeyState> keys_;    //!< 仮想キー入力状態

    ControlType control_type_ = ControlType::Player;    //!< 操作タイプ

    int action_key_ = KEY_INPUT_SPACE;    //!< アクション(攻撃・持ち上げ)キー

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
        arc(CEREAL_NVP(action_key_));

        arc(cereal::make_nvp("Component", cereal::base_class<Component>(this)));
    }
};

CEREAL_CLASS_VERSION(ComponentInput, 1);
