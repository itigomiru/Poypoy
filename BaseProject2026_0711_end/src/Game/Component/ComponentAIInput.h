//---------------------------------------------------------------------------
//!	@file	ComponentAIInput.h
//! @brief	AI入力
//---------------------------------------------------------------------------
#pragma once
#include <map>
#include <random>
#include <vector>
#include <System/Scene.h>
#include <System/Component/Component.h>

USING_PTR(ComponentAIInput);

class ComponentAIInput : public Component
{
public:
    BP_COMPONENT_DECL(ComponentAIInput, u8"AI入力");

    void Init() override;

    void Update() override;

    void GUI() override;

    //! @brief ターゲットをセットします
    ComponentAIInputPtr SetTarget(ObjectPtr target);

    //! @brief 現在のターゲットを取得します
    ObjectPtr GetTarget() const;

    //! @brief 移動キーをセットします
    ComponentAIInputPtr SetMoveKeys(int up, int down, int left, int right);

    //! @brief アクションキーをセットします
    ComponentAIInputPtr SetActionKeys(int attack, int defend, int dodge, int skill);

    //! @brief 距離判定をセットします
    ComponentAIInputPtr SetRange(float attack_range, float min_distance, float skill_range);

    //! @brief クールダウンをセットします(フレーム)
    ComponentAIInputPtr SetCooldown(u32 attack, u32 skill);

    //--------------------------------------------------------------------
    //! @name 仮想キー入力
    //! @details AIが出力したキー入力を物理入力(Input::)と同じ検証で読み出せます
    //@{

    //! @brief 指定キーの押下検証
    bool IsKey(int keyID) const;

    //! @brief 指定キーの1回だけ押下検証
    bool IsKeyDown(int keyID) const;

    //! @brief 指定キーが離された検証
    bool IsKeyUp(int keyID) const;

    //! @brief 指定キーの初回(1フレーム目)押下検証
    bool IsKeyOn(int keyID) const;

    //! @brief 指定キーが押されていない検証
    bool IsKeyRelease(int keyID) const;

    //! @brief 指定キーの長押し検証(指定フレーム以上押されている)
    bool IsKeyRepeat(int keyID, u32 frame = 2) const;

    //@}

private:
    enum class Action
    {
        Idle,
        Approach,
        Retreat,
        Attack,
        Defend,
        Dodge,
        Skill,
    };

    struct KeyState
    {
        u32 now_   = 0;
        u32 old_   = 0;
        u32 count_ = 0;
    };

    ObjectPtr SearchTarget();
    float3    GetFrontVector() const;
    float3    GetTargetDirection(const ObjectPtr& target, float& distance) const;
    void      AddMoveKeys(std::vector<int>& presses, const float3& dir) const;
    void      UpdateKeys(const std::vector<int>& presses);

    static const char* ActionName(Action action);

    Action action_ = Action::Idle;

    u32 attack_left_ = 0;    //!< 攻撃クールダウン残フレーム
    u32 skill_left_  = 0;    //!< スキルクールダウン残フレーム
    u32 defend_left_ = 0;    //!< 防御継続残フレーム
    u32 dodge_left_  = 0;    //!< 回避継続残フレーム

    float3 dodge_dir_ = {0, 0, 0};    //!< 回避の横移動方向

    std::map<int, KeyState> keys_;    //!< 仮想キー入力状態

    std::mt19937 rng_ = std::mt19937(std::random_device{}());

    ObjectWeakPtr target_;

    int key_up_    = KEY_INPUT_W;
    int key_down_  = KEY_INPUT_S;
    int key_left_  = KEY_INPUT_A;
    int key_right_ = KEY_INPUT_D;

    int key_attack_ = KEY_INPUT_SPACE;
    int key_defend_ = 0;
    int key_dodge_  = 0;
    int key_skill_  = 0;

    float attack_range_ = 8.0f;     //!< 攻撃可能距離
    float min_distance_ = 4.0f;     //!< これ以上近づかない距離
    float skill_range_  = 12.0f;    //!< スキル可能距離

    u32 attack_cooldown_ = 90;     //!< 攻撃クールダウン(フレーム)
    u32 skill_cooldown_  = 300;    //!< スキルクールダウン(フレーム)

    u32 defend_percent_ = 2;     //!< 防御発動確率(%)
    u32 defend_frames_  = 30;    //!< 防御継続フレーム
    u32 dodge_percent_  = 1;     //!< 回避発動確率(%)
    u32 dodge_frames_   = 15;    //!< 回避継続フレーム

    //--------------------------------------------------------------------
    //! @name Cereal処理
    //--------------------------------------------------------------------
    //@{

    //! @brief セーブ
    // @param arc アーカイバ
    // @param ver バージョン
    CEREAL_SAVELOAD(arc, ver)
    {
        arc(CEREAL_NVP(target_),

            CEREAL_NVP(key_up_),
            CEREAL_NVP(key_down_),
            CEREAL_NVP(key_left_),
            CEREAL_NVP(key_right_),

            CEREAL_NVP(key_attack_),
            CEREAL_NVP(key_defend_),
            CEREAL_NVP(key_dodge_),
            CEREAL_NVP(key_skill_),

            CEREAL_NVP(attack_range_),
            CEREAL_NVP(min_distance_),
            CEREAL_NVP(skill_range_),

            CEREAL_NVP(attack_cooldown_),
            CEREAL_NVP(skill_cooldown_),

            CEREAL_NVP(defend_percent_),
            CEREAL_NVP(defend_frames_),
            CEREAL_NVP(dodge_percent_),
            CEREAL_NVP(dodge_frames_));

        arc(cereal::make_nvp("Component", cereal::base_class<Component>(this)));
    }
};

CEREAL_CLASS_VERSION(ComponentAIInput, 1);
