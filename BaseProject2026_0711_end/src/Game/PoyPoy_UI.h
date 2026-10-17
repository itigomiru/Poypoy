//---------------------------------------------------------------------------
//!	@file	PoyPoy_UI.h
//! @brief	PoyPoy_UI
//---------------------------------------------------------------------------
#include <System/Scene.h>

namespace PoyPoy {
USING_PTR(PoyPoy_UI);

//! @brief プレイヤー人数
static constexpr int UI_MAX_PLAYERS = 4;

//! @brief プレイヤーUI情報
struct PlayerUI
{
    int  hp_     = 3;        //!< ハート数
    int  max_hp_ = 3;        //!< 最大ハート数
    bool retire_ = false;    //!< リタイア状態
};

//! @brief ゲーム画面に表示するUI
class PoyPoy_UI : public Object
{
public:
    BP_OBJECT_DECL(PoyPoy_UI, "PoyPoy::UI");

    //! @brief 初期化
    //! @return 初期化済み
    bool Init() override;

    void Update() override;

    void LateDraw() override;

    void GUI() override;

private:
    //! @brief タイマー表示
    void drawTimer(float adjust_x, float adjust_y);

    //! @brief プレイヤーパネル表示
    //! @param [in]    index       プレイヤー番号(0～)
    //! @param [in]    adjust_x    X座標補正
    //! @param [in]    adjust_y    Y座標補正
    void drawPlayerPanel(int index, float adjust_x, float adjust_y);

    //! @brief 顔アイコン表示
    //! @param [in]    x       左上X座標
    //! @param [in]    y       左上Y座標
    //! @param [in]    size    アイコンサイズ
    //! @param [in]    color   アイコン色
    //! @param [in]    retire  リタイア状態
    void drawFace(int x, int y, int size, unsigned int color, bool retire);

    //! @brief ハート表示
    //! @param [in]    x       左上X座標
    //! @param [in]    y       左上Y座標
    //! @param [in]    size    ハートサイズ
    //! @param [in]    color   ハート色
    void drawHeart(int x, int y, int size, unsigned int color);

    float timer_ = 83.0f;    //!< 残り時間(秒) ※開始は01:23

    PlayerUI players_[UI_MAX_PLAYERS];    //!< プレイヤー情報
};

}    // namespace PoyPoy
