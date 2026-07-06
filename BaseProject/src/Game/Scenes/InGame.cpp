//---------------------------------------------------------------------------
//!	@file	InGame.cpp
//! @brief	InGame
//---------------------------------------------------------------------------
#include "InGame.h"

namespace PoyPoy
{
	//! @brief 初期化
	//! @return 初期化済み
	bool InGame::Init()
	{
		__super::Init();
		// 最初に1回動作する
		// ただし trueを返さなければ Initに何回も来る仕様。
		Scene::Object::Create<Object>("MainCamera")->AddComponent<ComponentCamera>();
		return true;
	}

	void InGame::Update()
	{
		__super::Update();
		// 毎フレーム動作する
	}

	void InGame::Draw()
	{
		__super::Draw();
		// 毎フレーム動作する
	}

	void InGame::GUI()
	{
		__super::GUI();
		// 毎フレーム動作する
	}

}	 // namespace PoyPoy
