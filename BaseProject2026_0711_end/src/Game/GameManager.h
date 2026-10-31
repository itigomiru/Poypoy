//---------------------------------------------------------------------------
//!	@file	GameManager.h
//! @brief	対戦ゲームの試合進行管理
//---------------------------------------------------------------------------
#pragma once
#include <functional>
#include <vector>

namespace PoyPoy {

//! @brief ゲーム全体の進行状態・試合時間・参加プレイヤー・勝敗判定だけを扱う管理クラス。
//!        HPやダメージなどの戦闘まわりの情報は持たず、PlayerやBattleSystemからの
//!        通知とコールバックで連携する。
class GameManager
{
public:
    //! @brief ゲームの状態
    enum class GameState : u8 {
        Waiting,  //!< 試合開始前
        Playing,  //!< 試合中
        Finished, //!< 試合終了
    };

    //! @brief プレイヤー情報(HPなどの戦闘情報は持たない)
    struct PlayerInfo {
        u32  id;    //!< プレイヤーID
        bool alive; //!< 生存しているか
    };

    //! @brief 勝者なし(引き分け)やIDが見つからないことを表すプレイヤーID
    static constexpr u32 InvalidPlayerID = 0xFFFFFFFFu;

    //! @brief 時間切れの勝敗判定ルール。生存プレイヤー一覧から勝者IDを返し、
    //!        InvalidPlayerIDを返せば引き分け。残りHPなど基準のルールを後から差し込める
    std::function<u32(const std::vector<PlayerInfo>& players)> TimeoutRule;

    //! @brief ゲーム状態が変わったときに呼ばれるコールバック
    std::function<void(GameState state, u32 winner)> OnGameStateChanged;

    //! @brief 試合時間の設定(外部から設定する。試合開始前なら残り時間にも反映する)
    //! @param time 試合時間(秒)
    void SetMatchTime(float time);

    //! @brief 試合時間の取得
    //! @return 試合時間(秒)
    float GetMatchTime() const { return match_time_; }

    //! @brief 残り時間の取得
    //! @return 残り時間(秒)
    float GetRemainingTime() const { return remaining_time_; }

    //! @brief ゲーム状態の取得
    //! @return 現在のゲーム状態
    GameState GetGameState() const { return state_; }

    //! @brief 勝者の取得
    //! @return 勝者ID(試合終了前・引き分けはInvalidPlayerID)
    u32 GetWinnerID() const { return winner_id_; }

    //! @brief プレイヤーを参加させる(同じIDが既にいる場合は無視)
    //! @param id プレイヤーID
    void RegisterPlayer(u32 id);

    //! @brief 参加プレイヤー数の取得
    //! @return 参加プレイヤー数
    u32 GetPlayerCount() const { return static_cast<u32>(players_.size()); }

    //! @brief 生存プレイヤー数の取得
    //! @return 生存プレイヤー数
    u32 GetAliveCount() const;

    //! @brief プレイヤーが生存しているかの取得
    //! @param id プレイヤーID
    //! @return 生存しているか(参加していないプレイヤーは常にfalse)
    bool IsAlive(u32 id) const;

    //! @brief 試合開始(試合開始前からのみ開始し、タイマーを起動する)
    void StartGame();

    //! @brief 試合終了(生存者1名ならそのプレイヤー、それ以外は時間切れルールで勝敗を決める)
    void EndGame();

    //! @brief プレイヤーの死亡通知(死亡判定そのものは行わず、通知された死亡を反映するだけ)
    //! @param id 死亡したプレイヤーID
    void OnPlayerDeath(u32 id);

    //! @brief 毎フレームの更新(試合中のみ残り時間を減らし、0になったら時間切れ処理を行う)
    //! @param deltaTime 経過時間(秒)
    void Update(float deltaTime);

private:
    //! @brief 試合を終了して勝者を確定し、状態変化を通知する
    //! @param winner 勝者ID(InvalidPlayerIDで引き分け)
    void FinishGame(u32 winner);

    //! @brief 時間切れの勝敗判定を行う
    //! @return 勝者ID(ルール未設定・勝者なしはInvalidPlayerID)
    u32 ResolveTimeLimit() const;

    GameState           state_        = GameState::Waiting;     //!< 現在のゲーム状態
    float               match_time_     = 120.0f;               //!< 試合時間(秒)
    float               remaining_time_ = 120.0f;               //!< 残り時間(秒)
    u32                 winner_id_    = InvalidPlayerID;        //!< 勝者ID
    std::vector<PlayerInfo> players_;                           //!< 参加プレイヤー一覧
};

}    // namespace PoyPoy
