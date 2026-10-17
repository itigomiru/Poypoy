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

## 併用スキル：cyclomatic-complexity

[saurabhkumar8112/cyclomatic-complexity-skill](https://github.com/saurabhkumar8112/cyclomatic-complexity-skill) の `cyclomatic-complexity` Agent Skill も導入しています。関数ごとの循環複雑度を計測し、ガード節の抽出・関数分割・ルックアップテーブル化などで複雑な箇所をリファクタリングするスキルです。

「複雑度を下げて」「リファクタリングして」「関数を整理して」といった依頼で適用されます。リファクタリング後には before/after の複雑度表を出力します。

インストール済み場所：`.claude/skills/cyclomatic-complexity`

<skills_system priority="1">

## Available Skills

<!-- SKILLS_TABLE_START -->
<usage>
When users ask you to perform tasks, check if any of the available skills below can help complete the task more effectively. Skills provide specialized capabilities and domain knowledge.

How to use skills:
- Invoke: `npx openskills read <skill-name>` (run in your shell)
  - For multiple: `npx openskills read skill-one,skill-two`
- The skill content will load with detailed instructions on how to complete the task
- Base directory provided in output for resolving bundled resources (references/, scripts/, assets/)

Usage notes:
- Only use skills listed in <available_skills> below
- Do not invoke a skill that is already loaded in your context
- Each skill invocation is stateless
</usage>

<available_skills>

<skill>
<name>agentmemory-config</name>
<description>Set up and diagnose local agentmemory, including MCP, environment variables, ports, and feature flags. Use when getting started, checking a connection, enabling a feature, configuring auth, or explaining why a feature is off by default.</description>
<location>project</location>
</skill>

<skill>
<name>agentmemory-hooks</name>
<description>The agentmemory plugin hooks that capture observations automatically across the agent session lifecycle. Use when explaining how memory gets captured without manual saves, when debugging missing observations, or when tuning what gets recorded.</description>
<location>project</location>
</skill>

<skill>
<name>agentmemory-mcp-tools</name>
<description>Map of every agentmemory MCP tool, what each does, and its parameters. Use when choosing which memory tool to call, when a tool name or argument is unclear, or when answering what agentmemory can do via MCP.</description>
<location>project</location>
</skill>

<skill>
<name>agentmemory-rest-api</name>
<description>The agentmemory HTTP REST API surface, the primary protocol for talking to the memory server. Use when calling agentmemory over HTTP, when MCP is unavailable and you need a fallback, or when integrating a host that does not speak MCP.</description>
<location>project</location>
</skill>

<skill>
<name>commit-context</name>
<description>Trace a file, function, or line back to the agent session that produced its current commit. Use when the user asks "why is this code here", "what was the agent doing when this changed", "who wrote this", or wants context on a specific location in the codebase.</description>
<location>project</location>
</skill>

<skill>
<name>commit-history</name>
<description>List recent git commits linked to agent sessions, optionally filtered by branch or repo. Use when the user asks "show agent commits", "what has the agent shipped", "list linked commits", or wants commits with their session context.</description>
<location>project</location>
</skill>

<skill>
<name>cyclomatic-complexity</name>
<description>Refactor code to reduce cyclomatic complexity so it stays readable, maintainable, and aligned with the long-term vision of the codebase, not just optimized for AI comprehension. Use whenever the user asks to refactor, simplify, clean up, or review code quality; mentions complexity, maintainability, readability, spaghetti code, deeply nested logic, or god functions; or asks to check AI-generated code before merging. Also use proactively after writing any nontrivial function with heavy branching.</description>
<location>project</location>
</skill>

<skill>
<name>forget</name>
<description>Delete selected durable memories from agentmemory after showing them and getting explicit confirmation. Use when the user says "forget this", "delete memory", "remove that note", or wants to scrub specific data for privacy.</description>
<location>project</location>
</skill>

<skill>
<name>handoff</name>
<description>Resume the most recent agent session for the current working directory, leading with any unanswered question. Use when the user says "where were we", "resume", "handoff", "pick up where I left off", or starts a session with no fresh context.</description>
<location>project</location>
</skill>

<skill>
<name>lesson</name>
<description>Save a correction or hard-won rule as a confidence-weighted lesson that resurfaces before similar work. Use when the user corrects your approach, says "learn this", "always" or "never do X", or you notice yourself repeating a past mistake.</description>
<location>project</location>
</skill>

<skill>
<name>memory-discipline</name>
<description>The session loop that makes agentmemory pay off, recall before starting work, save at decision points, learn from corrections. Use when starting a nontrivial task, after settling a decision or debugging a gotcha, or whenever deciding if something belongs in memory.</description>
<location>project</location>
</skill>

<skill>
<name>natural-japanese</name>
<description>仕事の日本語文書を、自然で読みやすく、わかりやすい日本語で書く・直すためのスキル。議事録、調査レポート、顧客向けのレポート、社内ガイド、技術記事、PR の説明文、リサーチメモ、企画書、メール、スライド構成などの作成や推敲と、note やブログ、エッセイの執筆に使う。文字起こしから議事録を作るときにも使う。自然な日本語にしたい、読みやすくしたい、簡潔にしたい、結論から書きたいといった依頼のほか、読みづらい、わかりにくい、何を言っているかわからない、AI っぽい、AI 臭い、英語の直訳っぽい、日本語として不自然、違和感があるといった指摘にも使う。書き換えずに AI らしさを判定・採点してほしいという依頼や、自分の文体を学ばせたいという依頼にも対応する。</description>
<location>project</location>
</skill>

<skill>
<name>recall</name>
<description>Search agentmemory for past observations, sessions, and learnings about a topic using hybrid BM25 plus vector plus graph search. Use when the user says "recall", "what did we do about", "did we ever", "have we seen", or needs context from past sessions.</description>
<location>project</location>
</skill>

<skill>
<name>recap</name>
<description>Summarize the last N agent sessions for the current project, grouped by date, with highlight observations per session. Use when the user asks "recap", "what have we been doing", "today", "this week", or wants a rollup of recent work.</description>
<location>project</location>
</skill>

<skill>
<name>remember</name>
<description>Save an insight, decision, or learning to agentmemory's long-term storage with searchable concept tags. Use when the user says "remember this", "save this", "note that", "don't forget", or wants to preserve knowledge for future sessions.</description>
<location>project</location>
</skill>

<skill>
<name>sanitize-artifacts</name>
<description>Inspect and revise generated artifacts so they read as natural, standalone deliverables, without leaking prompt instructions, conversation history, implementation constraints, or production process artifacts into user-facing content.</description>
<location>project</location>
</skill>

<skill>
<name>session-history</name>
<description>Show what happened in recent past sessions on this project as a clean timeline. Use when the user asks "what did we do last time", "session history", "past sessions", or wants an overview of previous work.</description>
<location>project</location>
</skill>

<skill>
<name>write-agentmemory-skill</name>
<description>The house format and rules for writing or updating an agentmemory skill. Use when adding a new skill, restructuring an existing one, or reviewing a skill contribution for consistency.</description>
<location>project</location>
</skill>

</available_skills>
<!-- SKILLS_TABLE_END -->

</skills_system>
