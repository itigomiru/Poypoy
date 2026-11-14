//---------------------------------------------------------------------------
//!	@file	ComponentAIInput.cpp
//! @brief	AI入力
//---------------------------------------------------------------------------
#include <Game/Component/ComponentAIInput.h>
#include <Game/PoyPoy_Player.h>
#include "ComponentStateIdleWalk.h"

#include <limits>

void ComponentAIInput::Init()
{
    __super::Init();

    SetName<Component>("AI Input");

    control_type_ = ControlType::AI;

    if(!target_.lock())
        target_ = SearchTarget();
}

void ComponentAIInput::PreUpdate()
{
    __super::PreUpdate();

    if(Scene::IsPause())
        return;

    if(!GetStatus(StatusBit::Enable))
        return;

    // ターゲットを取得(いなければ再検索)
    auto target = GetTarget();
    if(!target) {
        target  = SearchTarget();
        target_ = target;
    }

    // クールダウンを減少させる
    if(attack_left_ > 0)
        attack_left_--;
    if(skill_left_ > 0)
        skill_left_--;
    if(defend_left_ > 0)
        defend_left_--;
    if(dodge_left_ > 0)
        dodge_left_--;

    // このフレームに押下する仮想キーのリスト
    std::vector<int> presses;

    if(target) {
        float  distance  = 0.0f;
        float3 to_target = GetTargetDirection(target, distance);
        float3 front     = GetFrontVector();
        bool   is_front  = (distance > 0.0f) ? ((float)dot(front, to_target) > 0.9f) : false;
        bool   in_attack = (distance <= attack_range_ && distance >= min_distance_);

        if(defend_left_ > 0 && key_defend_ != 0) {
            // 防御中
            action_ = Action::Defend;
            presses.push_back(key_defend_);
        }
        else if(dodge_left_ > 0) {
            // 回避中(横移動)
            action_ = Action::Dodge;
            if(key_dodge_ != 0)
                presses.push_back(key_dodge_);
            AddMoveKeys(presses, dodge_dir_);
        }
        else {
            // ターゲットが近い時に防御・回避を発動することがある
            if(distance <= attack_range_) {
                std::uniform_int_distribution<u32> percent(0, 99);
                if(key_defend_ != 0 && percent(rng_) < defend_percent_)
                    defend_left_ = defend_frames_;
                if(percent(rng_) < dodge_percent_) {
                    dodge_left_ = dodge_frames_;
                    float3 side = cross(to_target, {0, 1, 0});
                    dodge_dir_  = percent(rng_) < 50 ? side : -side;
                }
            }

            if(in_attack && is_front) {
                // 攻撃範囲内かつ正面: スキル・攻撃
                if(key_skill_ != 0 && distance <= skill_range_ && skill_left_ == 0) {
                    action_ = Action::Skill;
                    presses.push_back(key_skill_);
                    skill_left_ = skill_cooldown_;
                }
                else if(action_key_ != 0 && attack_left_ == 0) {
                    // アクションキーは基底(ComponentInput)が正。持ち上げ・投げと同じキー
                    action_ = Action::Attack;
                    presses.push_back(action_key_);
                    attack_left_ = attack_cooldown_;
                }
                else {
                    action_ = Action::Idle;
                }
            }
            else if(distance < min_distance_ && distance > 0.0f) {
                // 近すぎる: 後退
                action_ = Action::Retreat;
                AddMoveKeys(presses, -to_target);
            }
            else {
                // 遠い、または攻撃範囲内だが正面を向いていない: 接近
                action_ = Action::Approach;
                AddMoveKeys(presses, to_target);
            }
        }
    }
    else {
        action_ = Action::Idle;
    }

    // 仮想キー入力を物理入力と同じ状態遷移で更新する
    UpdateKeys(presses);
}

ComponentAIInputPtr ComponentAIInput::SetTarget(ObjectPtr target)
{
    target_ = target;
    return std::dynamic_pointer_cast<ComponentAIInput>(shared_from_this());
}

ObjectPtr ComponentAIInput::GetTarget() const
{
    return target_.lock();
}

ComponentAIInputPtr ComponentAIInput::SetMoveKeys(int up, int down, int left, int right)
{
    key_up_    = up;
    key_down_  = down;
    key_left_  = left;
    key_right_ = right;
    return std::dynamic_pointer_cast<ComponentAIInput>(shared_from_this());
}

ComponentAIInputPtr ComponentAIInput::SetActionKeys(int attack, int defend, int dodge, int skill)
{
    SetActionKey(attack);    // 持ち上げ・投げと同じキー
    key_defend_ = defend;
    key_dodge_  = dodge;
    key_skill_  = skill;
    return std::dynamic_pointer_cast<ComponentAIInput>(shared_from_this());
}

ComponentAIInputPtr ComponentAIInput::SetRange(float attack_range, float min_distance, float skill_range)
{
    attack_range_ = attack_range;
    min_distance_ = min_distance;
    skill_range_  = skill_range;
    return std::dynamic_pointer_cast<ComponentAIInput>(shared_from_this());
}

ComponentAIInputPtr ComponentAIInput::SetCooldown(u32 attack, u32 skill)
{
    attack_cooldown_ = attack;
    skill_cooldown_  = skill;
    return std::dynamic_pointer_cast<ComponentAIInput>(shared_from_this());
}

//---------------------------------------------------------------------------
// 内部処理
//---------------------------------------------------------------------------
ObjectPtr ComponentAIInput::SearchTarget()
{
    auto owner = GetOwnerPtr();
    if(!owner)
        return nullptr;

    // 自分以外のキャラで一番近いものをターゲットにする
    ObjectPtr best;
    float     best_distance = std::numeric_limits<float>::max();

    for(auto& character : Scene::Object::GetArray<PoyPoy::Player>()) {
        if(!character || character.get() == owner.get())
            continue;

        float3 dir = character->GetWorldTranslate() - owner->GetWorldTranslate();
        dir.y      = 0.0f;

        float distance = (float)length(dir);
        if(distance < best_distance) {
            best_distance = distance;
            best          = character;
        }
    }

    return best;
}

float3 ComponentAIInput::GetFrontVector() const
{
    auto owner = GetOwnerPtr();
    if(!owner)
        return {0, 0, -1};

    // モデルがある場合はモデルの前方(-Z軸)を前方とする
    if(auto mdl = owner->GetComponent<ComponentModel>()) {
        matrix m   = mdl->GetWorldMatrix();
        float3 vec = {-m._31, 0.0f, -m._33};
        if((float)length(vec) > 0.0f)
            return normalize(vec);
    }

    // モデルがない場合はオーナーの前方(-Z軸)を前方とする
    float3 vec = owner->GetWorldVectorAxisZ();
    vec.y      = 0.0f;
    if((float)length(vec) > 0.0f)
        return -normalize(vec);

    return {0, 0, -1};
}

float3 ComponentAIInput::GetTargetDirection(const ObjectPtr& target, float& distance) const
{
    auto owner = GetOwnerPtr();

    float3 dir = target->GetWorldTranslate() - owner->GetWorldTranslate();
    dir.y      = 0.0f;

    distance = (float)length(dir);
    if(distance > 0.0f)
        dir = normalize(dir);

    return dir;
}

void ComponentAIInput::AddMoveKeys(std::vector<int>& presses, const float3& dir) const
{
    auto owner = GetOwnerPtr();
    if(!owner || (float)length(dir) <= 0.0f)
        return;

    // 移動キーは ComponentStateIdleWalk が正。
    // IdleWalk が読まないキーIDを押しても反応しないため、
    // 読む側のキー設定と同じキーを押してやる
    int up    = key_up_;
    int down  = key_down_;
    int left  = key_left_;
    int right = key_right_;

    if(auto walk = owner->GetComponent<ComponentStateIdleWalk>()) {
        auto keys = walk->GetMoveKeys();
        up        = keys.up_;
        down      = keys.down_;
        left      = keys.left_;
        right     = keys.right_;
    }

    // 移動キーはオーナーのローカル軸で処理される(ComponentStateIdleWalkと同じ)
    // 欲しいワールド方向をローカル成分に変換してキーを選ぶ
    matrix mat = owner->GetMatrix();

    float lx = dot(dir, mat.axisX());
    float lz = dot(dir, mat.axisZ());

    if(lz < -0.1f)
        presses.push_back(up);
    if(lz > 0.1f)
        presses.push_back(down);
    if(lx > 0.1f)
        presses.push_back(left);
    if(lx < -0.1f)
        presses.push_back(right);
}

const char* ComponentAIInput::ActionName(Action action)
{
    switch(action) {
    case Action::Approach:
        return "接近";
    case Action::Retreat:
        return "後退";
    case Action::Attack:
        return "攻撃";
    case Action::Defend:
        return "防御";
    case Action::Dodge:
        return "回避";
    case Action::Skill:
        return "スキル";
    default:
        return "待機";
    }
}

void ComponentAIInput::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();

        if(ImGui::TreeNode("AI Input")) {
            // 有効/無効
            bool enable = GetStatus(StatusBit::Enable);
            if(ImGui::Checkbox(u8"有効", &enable))
                SetStatus(StatusBit::Enable, enable);

            // GUI上でオーナーから自分を削除します
            if(ImGui::Button(u8"削除"))
                GetOwner()->RemoveComponent(shared_from_this());

            auto target = GetTarget();

            float distance = 0.0f;
            if(target)
                (void)GetTargetDirection(target, distance);

            ImGui::Text(u8"ターゲット: %s", target ? target->GetName().data() : u8"なし");
            ImGui::Text(u8"距離: %.2f", distance);
            ImGui::Text(u8"行動: %s", ActionName(action_));
            ImGui::Text(u8"攻撃CD残: %u / スキルCD残: %u", attack_left_, skill_left_);

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

CEREAL_REGISTER_TYPE(ComponentAIInput)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentAIInput)
