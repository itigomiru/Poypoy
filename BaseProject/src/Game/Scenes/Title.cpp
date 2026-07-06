//---------------------------------------------------------------------------
//!	@file	Title.cpp
//! @brief	Title
//---------------------------------------------------------------------------
#include "Title.h"
#include "Pick.h"

namespace PoyPoy
{
	//! @brief 初期化
	//! @return 初期化済み
	bool Title::Init()
	{
		__super::Init();
		// 最初に1回動作する
		// ただし trueを返さなければ Initに何回も来る仕様。
		return true;
	}

	void Title::Update()
	{
		__super::Update();
		// 毎フレーム動作する

		//if(Input::IsKeyDown(KEY_INPUT_SPACE))
			//Scene::Change(GetScene<Pick>());
	}

	void Title::Draw()
	{
		__super::Draw();
		// 毎フレーム動作する
	}

	void Title::GUI()
	{
		__super::GUI();
		// 毎フレーム動作する
	}

}	 // namespace PoyPoy
