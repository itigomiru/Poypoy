//---------------------------------------------------------------------------
//!	@file	GameManager.cpp
//! @brief	対戦ゲームの試合進行管理
//---------------------------------------------------------------------------
#include "GameManager.h"

namespace PoyPoy {

void GameManager::SetMatchTime(float time)
{
    if(time < 0.0f)
        time = 0.0f;

    match_time_ = time;

    // 試合開始前なら残り時間にも反映する
    if(state_ == GameState::Waiting)
        remaining_time_ = time;
}

void GameManager::RegisterPlayer(u32 id)
{
    for(auto& player : players_) {
        if(player.id == id)
            return;
    }
    players_.push_back({id, true});
}

u32 GameManager::GetAliveCount() const
{
    u32 count = 0;
    for(auto& player : players_) {
        if(player.alive)
            count++;
    }
    return count;
}

bool GameManager::IsAlive(u32 id) const
{
    for(auto& player : players_) {
        if(player.id == id)
            return player.alive;
    }
    return false;
}

void GameManager::StartGame()
{
    // 試合開始前からのみ開始し、タイマーを起動する
    if(state_ != GameState::Waiting)
        return;

    state_          = GameState::Playing;
    remaining_time_ = match_time_;
    winner_id_      = InvalidPlayerID;

    if(OnGameStateChanged)
        OnGameStateChanged(state_, winner_id_);
}

void GameManager::EndGame()
{
    if(state_ == GameState::Finished)
        return;

    // 終了時点の生存者で勝敗を決める(生存者1名ならそのプレイヤー、それ以外は時間切れルール)
    u32 winner = InvalidPlayerID;
    if(GetAliveCount() == 1) {
        for(auto& player : players_) {
            if(player.alive) {
                winner = player.id;
                break;
            }
        }
    }
    else {
        winner = ResolveTimeLimit();
    }

    FinishGame(winner);
}

void GameManager::OnPlayerDeath(u32 id)
{
    if(state_ != GameState::Playing)
        return;

    bool found = false;
    for(auto& player : players_) {
        if(player.id == id && player.alive) {
            player.alive = false;
            found        = true;
            break;
        }
    }
    if(!found)
        return;

    // 生存プレイヤーが1人になったらそのプレイヤーの勝ち、0人なら引き分けで終了する
    u32 alive = GetAliveCount();
    if(alive == 1) {
        u32 winner = InvalidPlayerID;
        for(auto& player : players_) {
            if(player.alive) {
                winner = player.id;
                break;
            }
        }
        FinishGame(winner);
    }
    else if(alive == 0) {
        FinishGame(InvalidPlayerID);
    }
}

void GameManager::Update(float deltaTime)
{
    // 試合中のみ残り時間を減らす(試合開始前と試合終了後はタイマーを停止)
    if(state_ != GameState::Playing)
        return;

    remaining_time_ -= deltaTime;

    // 残り時間が0になったら時間切れとして試合を終了する
    if(remaining_time_ <= 0.0f) {
        remaining_time_ = 0.0f;
        FinishGame(ResolveTimeLimit());
    }
}

void GameManager::FinishGame(u32 winner)
{
    state_     = GameState::Finished;
    winner_id_ = winner;

    if(OnGameStateChanged)
        OnGameStateChanged(state_, winner_id_);
}

u32 GameManager::ResolveTimeLimit() const
{
    // 時間切れのルールは外部から差し替えられる(未設定なら引き分け)
    if(TimeoutRule)
        return TimeoutRule(players_);

    return InvalidPlayerID;
}

}    // namespace PoyPoy
