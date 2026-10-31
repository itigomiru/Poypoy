# BaseProject ルール(Kilo Code 用)

C++ / DXLib / Direct3D11 / Effekseer / ImGui / JoltPhysics / cereal のゲームプロジェクト。
設計方針は「Scene → Object → Component」の3層。コードを書く前に、どの層の責務かを判断すること。

## 1. 設計原則
- **責務分離**: Scene は流れと生成、Object は実体と識別、Component は振る舞いとデータ。層をまたぐ処理を1か所に書かない。
- **継承より合成**: 機能追加は新しい Component で行い、Object の継承ツリーを深くしない。
- **共通化**: 同じ処理が2か所に出たら基底クラスか共通関数にまとめる(例: Item 系は `ItemBase` に統合)。
- **データ駆動**: パス・数値・設定値はコードに散らさず、定数、`data/` 配下のファイル、`Game.ini` に置く。マジックナンバー禁止。
- **所有権の明示**: 生ポインタを持たず、`USING_PTR` で作る `Ptr` / `WeakPtr` / `UniquePtr` を使う。リソースは RAII で解放する(`Exit()` での明示解放が必要なものは必ず対にする)。

## 2. 型登録(必須)
| 種類 | マクロ | 解説文字列 |
|---|---|---|
| Scene / class 系 | `BP_CLASS_DECL(クラス名, u8"解説")` | `u8` 付き |
| Object | `BP_OBJECT_DECL(クラス名, "解説")` | 通常文字列 |
| Component | `BP_COMPONENT_DECL(クラス名, u8"解説")` | `u8` 付き |

- 旧マクロ(`BP_*_TYPE`)は使わない。見つけたら置換する。
- 解説文字列はクラスごとに固有にする(コピペで他クラスと重複させない)。
- クラス名の直後に `USING_PTR(クラス名)` を置く。

## 3. Scene / Object / Component の規約
- Scene は `Scene::Base` を継承し、シーンごとに namespace を切る。
- シーン遷移は `Scene::Change(Scene::GetScene<T>())` に統一する。
- Object の生成・取得・解放は `Scene::Object::Create<T>()` / `Get<T>()` / `Release()` を使う。
- Object の `Init()` を override したら必ず `Super::Init()` を呼ぶ(呼ばないと assert)。
- Component の `Init()` / `Update()` を override したら基底を呼ぶ。
- 衝突相手の判定は `GetOwner()->GetNameDefault()` で行う(`GetName()` は使わない)。
- Component の setter は自身の `Ptr` を返し、`AddComponent<T>(...)->SetXxx(...)` の連結で書く。
- すべての派生クラスに `override` を付ける。

## 4. シリアライズ(cereal)
- Component は `CEREAL_SAVELOAD` を持ち、基底は `cereal::make_nvp("Component", cereal::base_class<Component>(this))` で保存する。
- 保存項目を変えたら `CEREAL_CLASS_VERSION` を上げ、旧データを読めるようにバージョン分岐を書く。
- ポリモーフィックな Component は cpp 末尾で `CEREAL_REGISTER_TYPE` と `CEREAL_REGISTER_POLYMORPHIC_RELATION` を書く。
- Scene の保存形式を変えたら `SCENE_VERSION` を更新する。

## 5. コーディング規約
- 基本型は `s32` / `u32` / `f32` などのエイリアスを使う。
- 浮動小数リテラルには `f` を付ける。
- メンバ変数は末尾 `_`(例: `is_get_`)。`thisRelease` のような規約外の名前は作らない。
- `#pragma once` を先頭に置く(`#include` より前)。
- ファイル冒頭コメントは、そのファイル名と一致させる(コピペ元の名前を残さない)。
- 各ソースは `Precompile.h` が強制インクルードされる前提で書く(`#include "Precompile.h"` は書かない)。
- 書式は `src/.clang-format-reference` に従う。文字コードは UTF-8 BOM、改行は C++ が LF、`.bat` / `.cmd` が CRLF。

## 6. パスと設定
- ランタイムデータは `data/` 配下、保存データは `data/_save/` 配下に置く。
- `IniFileLib("Game.ini")` は自動で `data/` が付くため、`data/` を重ねて書かない。
- モデルパスと画像パスを同じ変数で兼用しない(`GetModelPath()` と `GetImagePath()` を混同しない)。

## 7. ビルドと確認
- ビルドは `& ".\@build.cmd" Debug`(`cmd /c` 経由は使わない)。成功時は `Build successful.`
- `.build`、`.vs`、`.vscode`、`data/_save`、`data/Game.ini`、`src/.clang-format` は Git 管理外。生成物を編集・コミットしない。
- 作業の完了条件: ①Debug ビルドが通る ②警告を増やさない ③上記の型登録・規約違反がない。

## 8. 作業の進め方
- 変更前に、関連する既存クラスの実装を読み、既存の書き方に合わせる。
- 1回の変更は1つの目的に絞る。無関係なリファクタを混ぜない。
- 規約違反の既存コード(`ItemBase` の `BP_OBJECT_DECL` 欠落、`Paint` / `Picture` の基底や解説文字列など)は、触るついでに直す。ただし直した内容を報告する。
- 未実装・未接続の箇所(`Loading`、`ItemView`、`Clear` の遷移)を勝手に仮実装しない。必要なら確認する。
- 不明点は推測で進めず、選択肢を挙げて質問する。
