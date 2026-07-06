//---------------------------------------------------------------------------
//!	@file	Result.h
//! @brief	Result
//---------------------------------------------------------------------------
#include <System/Scene.h>

namespace PoyPoy
{

	class Result : public Scene::Base
	{
	public:
		BP_CLASS_DECL(Result, u8"Result");

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
