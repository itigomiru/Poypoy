//---------------------------------------------------------------------------
//!	@file	PoyPoy_UI.cpp
//! @brief	PoyPoy_UI
//---------------------------------------------------------------------------
#include "PoyPoy_UI.h"

#include <System/Scene.h>
#include <System/SystemMain.h>

namespace PoyPoy {

namespace {
//! 表示基準解像度
constexpr int DESIGN_W = 1280;
constexpr int DESIGN_H = 720;

//! タイマー表示位置/サイズ
constexpr int TIMER_X = 24;
constexpr int TIMER_Y = 20;
constexpr int TIMER_W = 160;
constexpr int TIMER_H = 76;

//! プレイヤーパネルの余白/間隔
constexpr int PANEL_MARGIN = 28;
constexpr int PANEL_GAP    = 24;
constexpr int PANEL_H      = 108;

//! パネル内要素サイズ
constexpr int FACE_SIZE = 60;    //!< 顔アイコンサイズ

//! パネル内余白
constexpr int PANEL_PADDING = 14;

//! 文字サイズ
constexpr int FONT_TIME   = 40;    //!< 残り時間
constexpr int FONT_LABEL  = 22;    //!< 残り時間/プレイヤー番号
constexpr int FONT_HP     = 26;    //!< HP数値
constexpr int FONT_RETIRE = 22;    //!< リタイア文字

//! パネル地色
const u32 COLOR_PANEL      = GetColor(16, 24, 64);
const u32 COLOR_PANEL_LINE = GetColor(220, 228, 255);
const u32 COLOR_TIME       = GetColor(255, 255, 255);
const u32 COLOR_TIME_CAP   = GetColor(140, 180, 255);
const u32 COLOR_LABEL      = GetColor(255, 200, 40);
const u32 COLOR_RETIRE     = GetColor(220, 80, 255);
const u32 COLOR_WHITE      = GetColor(255, 255, 255);

//! プレイヤーカラー
const u32 PLAYER_COLORS[UI_MAX_PLAYERS] = {
    GetColor(200, 60, 60),     //!< 1P
    GetColor(60, 140, 220),    //!< 2P
    GetColor(80, 180, 90),     //!< 3P
    GetColor(230, 160, 40),    //!< 4P
};
}    // namespace

bool PoyPoy_UI::Init()
{
    // 最初に1回動作する
    // ただし trueを返さなければ Initに何回も来る仕様。
    Super::Init();

    SetName("UI");

    for(int i = 0; i < UI_MAX_PLAYERS; ++i) {
        auto path = std::format("data/Game/UI/face{}.png", i + 1);
        loadFaceGraph(i, path.c_str());
    }

    return true;
}

void PoyPoy_UI::Exit()
{
    for(int i = 0; i < UI_MAX_PLAYERS; ++i) {
        if(players_[i].faceGraph_ >= 0) {
            DeleteGraph(players_[i].faceGraph_);
            players_[i].faceGraph_ = -1;
        }
    }

    Super::Exit();
}

void PoyPoy_UI::Update()
{
    // 残り時間を減らす
    timer_ -= GetDeltaTime();
    if(timer_ < 0.0f)
        timer_ = 0.0f;

    // HP表示対象のキャラクターを毎フレーム取得(1P,2P,...の順)
    characters_.clear();
    for(auto& character : Scene::Object::GetArray<Character>()) {
        if(characters_.size() >= UI_MAX_PLAYERS)
            break;
        characters_.push_back(character);
    }
}

void PoyPoy_UI::LateDraw()
{
    float ADJUST_X = (WINDOW_W / (float)DESIGN_W);
    float ADJUST_Y = (WINDOW_H / (float)DESIGN_H);

    // タイマー
    drawTimer(ADJUST_X, ADJUST_Y);

    // プレイヤーパネル
    for(int i = 0; i < UI_MAX_PLAYERS; ++i) drawPlayerPanel(i, ADJUST_X, ADJUST_Y);

    // フォントサイズを元に戻す
    SetFontSize(16);
}

void PoyPoy_UI::drawTimer(float adjust_x, float adjust_y)
{
    int x = static_cast<int>(TIMER_X * adjust_x);
    int y = static_cast<int>(TIMER_Y * adjust_y);
    int w = static_cast<int>(TIMER_W * adjust_x);
    int h = static_cast<int>(TIMER_H * adjust_y);

    // パネル
    DrawRoundRect(x, y, x + w, y + h, 12, 12, COLOR_PANEL, TRUE);
    DrawRoundRect(x, y, x + w, y + h, 12, 12, COLOR_PANEL_LINE, FALSE);

    // 残り時間 (MM:SS)
    int  total = static_cast<int>(timer_);
    int  min   = total / 60;
    int  sec   = total % 60;
    auto text  = std::format("{:02d}:{:02d}", min, sec);

    SetFontSize(static_cast<int>(FONT_TIME * adjust_x));
    DrawString(x + static_cast<int>(16 * adjust_x), y + static_cast<int>(8 * adjust_y), text.c_str(), COLOR_TIME, COLOR_PANEL);

    // "TIME" 表示
    SetFontSize(static_cast<int>(FONT_LABEL * adjust_x));
    DrawString(x + static_cast<int>(16 * adjust_x), y + static_cast<int>(50 * adjust_y), "TIME", COLOR_TIME_CAP, COLOR_PANEL);
}

void PoyPoy_UI::drawPlayerPanel(int index, float adjust_x, float adjust_y)
{
    int panel_w = (WINDOW_W - static_cast<int>(PANEL_MARGIN * adjust_x) * 2 - static_cast<int>(PANEL_GAP * adjust_x) * (UI_MAX_PLAYERS - 1)) / UI_MAX_PLAYERS;
    int panel_h = static_cast<int>(PANEL_H * adjust_y);

    int px = static_cast<int>(PANEL_MARGIN * adjust_x) + index * (panel_w + static_cast<int>(PANEL_GAP * adjust_x));
    int py = WINDOW_H - static_cast<int>(PANEL_MARGIN * adjust_y) - panel_h;

    const PlayerUI& player = players_[index];

    // パネル
    DrawRoundRect(px, py, px + panel_w, py + panel_h, 14, 14, COLOR_PANEL, TRUE);
    DrawRoundRect(px, py, px + panel_w, py + panel_h, 14, 14, COLOR_PANEL_LINE, FALSE);

    // プレイヤー番号
    SetFontSize(static_cast<int>(FONT_LABEL * adjust_x));
    auto label = std::format("{}P", index + 1);
    DrawString(px + static_cast<int>(PANEL_PADDING * adjust_x), py + static_cast<int>(8 * adjust_y), label.c_str(), COLOR_LABEL, COLOR_PANEL);

    // 顔アイコン
    int face_size = static_cast<int>(FACE_SIZE * adjust_x);
    int face_x    = px + static_cast<int>(PANEL_PADDING * adjust_x);
    int face_y    = py + static_cast<int>(34 * adjust_y);
    drawFace(face_x, face_y, face_size, PLAYER_COLORS[index], player.retire_, player.faceGraph_);

    // HP数値 / リタイア表示
    int info_x = face_x + face_size + static_cast<int>(14 * adjust_x);
    int info_y = face_y;

    auto character = (index < static_cast<int>(characters_.size())) ? characters_[index].lock() : nullptr;

    if(player.retire_) {
        // リタイアは文字と×表示
        SetFontSize(static_cast<int>(FONT_RETIRE * adjust_x));
        DrawString(info_x, info_y, "RETIRE!", COLOR_RETIRE, COLOR_PANEL);
    }
    else if(character) {
        // 実HPを数値表示「現在 / 最大」
        int area_x = info_x;
        int area_w = px + panel_w - static_cast<int>(PANEL_PADDING * adjust_x) - area_x;
        int area_h = py + panel_h - static_cast<int>(PANEL_PADDING * adjust_y) - info_y;

        auto text = std::format("{:.0f} / {:.0f}", character->GetHP(), character->GetMaxHP());
        int  font = static_cast<int>(FONT_HP * adjust_x);
        SetFontSize(font);
        int text_w = GetDrawStringWidth(text.c_str(), font);
        int text_x = area_x + (area_w - text_w) / 2;
        int text_y = info_y + (area_h - font) / 2;

        // 体力が少ないほど赤へ寄る
        float ratio = (character->GetMaxHP() > 0.0f) ? std::clamp(character->GetHP() / character->GetMaxHP(), 0.0f, 1.0f) : 0.0f;
        u32   color = GetColor(255, static_cast<u8>(ratio * 255), static_cast<u8>(ratio * 255));

        DrawString(text_x, text_y, text.c_str(), color, COLOR_PANEL);
    }
}

void PoyPoy_UI::drawFace(int x, int y, int size, unsigned int color, bool retire, int faceGraph)
{
    if(faceGraph >= 0) {
        DrawExtendGraph(x, y, x + size, y + size, faceGraph, TRUE);
    }
    else {
        // 顔アイコン
        DrawRoundRect(x, y, x + size, y + size, size / 6, size / 6, color, TRUE);

        // 目
        int eye = size / 9;
        DrawCircle(x + size / 2 - eye * 2, y + size / 2 - eye, eye, COLOR_WHITE, TRUE);
        DrawCircle(x + size / 2 + eye * 2, y + size / 2 - eye, eye, COLOR_WHITE, TRUE);

        // 口
        int mx = x + size / 2;
        int my = y + size / 2 + eye * 2;
        DrawOval(mx, my, size / 5, size / 10, COLOR_WHITE, TRUE);
    }

    if(retire) {
        // リタイアは×を表示
        int m = size / 8;
        DrawLine(x + m, y + m, x + size - m, y + size - m, COLOR_RETIRE, 4);
        DrawLine(x + size - m, y + m, x + m, y + size - m, COLOR_RETIRE, 4);
    }
}

bool PoyPoy_UI::loadFaceGraph(int index, const char* path)
{
    int handle = LoadGraph(path);
    if(handle >= 0) {
        players_[index].faceGraph_ = handle;
        return true;
    }
    return false;
}

void PoyPoy_UI::GUI()
{
    Super::GUI();

    ImGui::DragFloat(u8"残り時間(秒)", &timer_, 1.0f, 0.0f, 3600.0f);

    for(int i = 0; i < UI_MAX_PLAYERS; ++i) {
        std::string name = std::format("{}P", i + 1);
        if(ImGui::TreeNode(name.c_str())) {
            if(i < static_cast<int>(characters_.size())) {
                if(auto character = characters_[i].lock()) {
                    ImGui::Text(u8"HP %.0f / %.0f", character->GetHP(), character->GetMaxHP());
                }
                else {
                    ImGui::Text(u8"HP - / -");
                }
            }
            else {
                ImGui::Text(u8"HP - / -");
            }
            ImGui::Checkbox(u8"リタイア", &players_[i].retire_);
            ImGui::TreePop();
        }
    }
}

}    // namespace PoyPoy
