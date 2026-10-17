# AGENTS.md

このプロジェクトは、AI 生成の日本語ドキュメントを読みやすく推敲するため
[coji/natural-japanese](https://github.com/coji/natural-japanese) Agent Skill を利用します。

## 使い方

エージェントに「読みやすくして」「議事録にして」「AI っぽいところを直して」のように依頼すると、natural-japanese スキルが適用されます。

自然さの点数（0〜100）と理由だけを知りたいときは、以下のように指定してください：

```
/natural-japanese score <ファイル>
```

## インストール

未導入の場合は以下でインストールできます：

```bash
npx openskills install coji/natural-japanese
npx openskills sync
```

または（Claude Code の場合）：

```bash
npx skills add coji/natural-japanese
```

## 守ること

- 元の文書にない事実・数値・意見・方針を足さない
- 確信の度合いを変えない

## 苦手なこと

- 英語の言い方を1語ずつ訳した熟語や、動作を名詞にまとめた言い回しは半分ほど見逃す
- effort（推論量）を下げると手順の一部を省いて見逃しが増える

## 併用スキル：sanitize-artifacts

[kotek-7/dotfiles](https://github.com/kotek-7/dotfiles) の `sanitize-artifacts` Agent Skill も導入しています。生成物（議事録、レポート、PR 説明文、コミットメッセージなど）にプロンプト指示や会話履歴、実装上の制約、制作過程の痕跡が混入していないか検査・除去するスキルです。

「生成物をきれいにして」「プロンプトの痕跡を消して」「読者向けに直して」といった依頼で適用されます。自然-japanese と組み合わせて、日本語の推敲とアーティファクトのサニタイズの両方を行えます。

インストール済み場所：`.claude/skills/sanitize-artifacts`
