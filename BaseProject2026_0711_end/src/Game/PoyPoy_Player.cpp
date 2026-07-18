//---------------------------------------------------------------------------
//!	@file	PoyPoy_Player.cpp
//! @brief	PoyPoy_Player
//---------------------------------------------------------------------------

#include "PoyPoy_Player.h"
#include "PoyPoy_Bullet.h"
#include "InGame.h"
#include "Component/ComponentStateIdleWalk.h"
#include "Component/ComponentCameraController.h"
#include "Component/ComponentStateThrow.h"
#include "Component/ComponentPlayerState.h"

namespace PoyPoy
{
	//! @brief 初期化
	//! @return 初期化終了
	bool Player::Init()
	{
		Super::Init();

		SetName("Player");

		//auto player = Scene::Object::Create<Object>("Player");
		AddComponent<ComponentModel>("data/Game/Models/Player/untitled.mv1")
			->SetAnimation({
				{ "idle",	 "data/Game/Models/Player/Anims/Idle.mv1", 1, 1.0f}, // Idle
				{ "walk",	 "data/Game/Models/Player/Anims/Walk.mv1", 1, 1.0f}, // Walk
				{"throw", "data/Game/Models/Player/Anims/Throw.mv1", 1, 1.0f}, // Throw
		})
			->SetScaleAxisXYZ({0.1f, 0.1f, 0.1f});

		SetTranslate({0, 5, 0});

		AddComponent<ComponentPlayerState>();

		AddComponent<ComponentCameraController>();

		auto col = AddComponent<ComponentCollisionCapsule>()	//
					   ->SetRadius(3.0f)
					   ->SetHeight(13.0f);

		col->UseGravity();
		col->SetCollisionGroup(ComponentCollision::CollisionGroup::PLAYER);

		return true;
	}

	void Player::Update()
	{
#if 0
		if(Input::IsKeyDown(KEY_INPUT_SPACE))
		{
			const float3 up_len = {0.0f, 5.0f, 0.0f};
			float3		 vec	= {0.0f, 1.0f, 0.0f};
			if(auto model = GetComponent<ComponentModel>())
			{
				constexpr float front_len = 5.0f;

				vec = -model->GetWorldVectorAxisZ() * front_len;
			}

			auto bullet = Scene::Object::Create<Bullet>();
			bullet->SetDirection(vec);
			bullet->SetTranslate(GetTranslate() + up_len + vec);
		}
#endif
	}

}	 // namespace PoyPoy
