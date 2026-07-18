//---------------------------------------------------------------------------
//!	@file	PoyPoy_GameOver.h
//! @brief	PoyPoy_GameOver
//---------------------------------------------------------------------------
#include <System/Scene.h>

namespace PoyPoy
{

	class PoyPoy_GameOver : public Scene::Base
	{
	public:
		BP_CLASS_DECL(PoyPoy_GameOver, u8"PoyPoyゲームオーバー");
		//! @brief 初期化
		//! @return 初期化済み
		bool Init() override;

		void Update() override;

		void LateDraw() override;

		void GUI() override;

	private:
		int x_	  = 20;
		int y_	  = 180;
		int size_ = 315;
	};

}	 // namespace PoyPoy
