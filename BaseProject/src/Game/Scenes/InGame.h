//---------------------------------------------------------------------------
//!	@file	InGame.h
//! @brief	InGame
//---------------------------------------------------------------------------
#include <System/Scene.h>

namespace PoyPoy
{

	class InGame : public Scene::Base
	{
	public:
		BP_CLASS_DECL(InGame, u8"InGame");

		//! @brief 初期化
		//! @return 初期化済み
		bool Init() override;

		void Update() override;

		//! @brief 描画
		void Draw() override;

		//! @brief GUI表示
		void GUI() override;

	private:
	};

}	 // namespace PoyPoy
