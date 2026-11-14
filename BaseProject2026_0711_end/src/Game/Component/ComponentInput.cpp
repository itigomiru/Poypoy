//---------------------------------------------------------------------------
//!	@file	ComponentInput.cpp
//! @brief	入力ソース基底
//---------------------------------------------------------------------------
#include <Game/Component/ComponentInput.h>
#include <Game/Component/ComponentAIInput.h>
#include <Game/Component/ComponentPlayerInput.h>

void ComponentInput::Init()
{
    __super::Init();

    SetName<Component>("Input");
}

//---------------------------------------------------------------------------
// 入力検証(仮想キー)
// PlayerInputはここを物理入力へ差し替えます
//---------------------------------------------------------------------------
bool ComponentInput::IsKey(int keyID) const
{
    if(auto it = keys_.find(keyID); it != keys_.end())
        return it->second.now_ != 0;
    return false;
}

bool ComponentInput::IsKeyDown(int keyID) const
{
    if(auto it = keys_.find(keyID); it != keys_.end())
        return it->second.now_ != 0 && it->second.old_ == 0;
    return false;
}

bool ComponentInput::IsKeyUp(int keyID) const
{
    if(auto it = keys_.find(keyID); it != keys_.end())
        return it->second.now_ == 0 && it->second.old_ != 0;
    return false;
}

bool ComponentInput::IsKeyOn(int keyID) const
{
    if(auto it = keys_.find(keyID); it != keys_.end())
        return it->second.count_ == 1;
    return false;
}

bool ComponentInput::IsKeyRelease(int keyID) const
{
    if(auto it = keys_.find(keyID); it != keys_.end())
        return it->second.count_ == 0;
    return true;
}

bool ComponentInput::IsKeyRepeat(int keyID, u32 frame) const
{
    if(auto it = keys_.find(keyID); it != keys_.end())
        return it->second.count_ >= frame;
    return false;
}

//---------------------------------------------------------------------------
// アクションキー
//---------------------------------------------------------------------------
void ComponentInput::SetActionKey(int key)
{
    action_key_ = key;
}

//---------------------------------------------------------------------------
// 入力ソースの設定/取得
//---------------------------------------------------------------------------
ComponentInputPtr ComponentInput::Get(Object* owner)
{
    assert(owner && "入力ソースはオブジェクトに付与します。ownerがnullです");
    if(!owner)
        return nullptr;

    if(auto input = owner->GetComponent<ComponentInput>())
        return input;

    // 入力ソース未設定のキャラには物理入力を1体ごとに付与する
    return owner->AddComponent<ComponentPlayerInput>();
}

ComponentInputPtr ComponentInput::Create(Object* owner, ControlType type)
{
    if(!owner)
        return nullptr;

    if(auto current = owner->GetComponent<ComponentInput>()) {
        // 同じ操作タイプなら作り直さない
        if(current->GetControlType() == type)
            return current;

        // 操作タイプの切り替えは設定されている入力ソースを置き換える
        owner->RemoveComponent(current);
    }

    if(type == ControlType::AI)
        return owner->AddComponent<ComponentAIInput>();

    return owner->AddComponent<ComponentPlayerInput>();
}

//---------------------------------------------------------------------------
// 内部処理
//---------------------------------------------------------------------------
void ComponentInput::UpdateKeys(const std::vector<int>& presses)
{
    // このフレーム押下されていないキーは離された扱いにする
    // (物理入力は毎フレーム現在キー的状态を読み直すため、離すと0に戻る)
    for(auto& [key, state] : keys_) {
        state.old_ = state.now_;
        state.now_ = 0;
    }

    // このフレーム押下されたキー
    for(auto key : presses) keys_[key].now_ = 1;

    // 押されているフレーム数を更新(物理入力と同じ扱い)
    for(auto& [key, state] : keys_) {
        if(state.now_ != 0) {
            state.count_++;
            if(state.count_ >= CHAR_MAX)
                state.count_ = CHAR_MAX;
        }
        else {
            state.count_ = 0;
        }
    }
}

CEREAL_REGISTER_TYPE(ComponentInput)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentInput)
