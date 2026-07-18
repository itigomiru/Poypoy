#pragma once
#include "ComponentStateIdleWalk.h"
#include <Game/Component/ComponentStateThrow.h>
#include <Game/PoyPoy_Bullet.h>

void ComponentStateThrow::Init()
{
	__super::Init();

	SetName<Component>("State Throw");

	const float3 up_len = {0.0f, 5.0f, 0.0f};
	float3		 vec	= {0.0f, 1.0f, 0.0f};
	if(auto model = GetOwner()->GetComponent<ComponentModel>())
	{
		constexpr float front_len = 5.0f;

		vec = -model->GetWorldVectorAxisZ() * front_len;

		model->PlayAnimation("throw");
	}

	auto bullet = Scene::Object::Create<PoyPoy::Bullet>();
	bullet->SetDirection(vec);
	bullet->SetTranslate(GetOwner()->GetTranslate() + up_len + vec);

	bullet_ = bullet;

	auto attach = bullet->AddComponent<ComponentAttachModel>();
	attach->SetAttachObject("Player", "mixamorig:RightHand");
	attach->SetAttachOffset({12, 17, 0});
}

void ComponentStateThrow::Update()
{
	__super::Update();

	// GetAnimationTimeを使って書き換え
	if(!threw_ && GetAnimationTime() > 0.8)
	{
		if(auto bullet = bullet_.lock())
		{
			bullet->RemoveComponent<ComponentAttachModel>();
			threw_ = true;
		}
	}

	if(threw_)
	{
		if(Input::IsKeyDown(KEY_INPUT_SPACE))
			ChangeState<ComponentStateThrow>();
	}

	// 理想的なものに切り替えた
	ChangeStateWhenAnimEnd<ComponentStateIdleWalk>();
}

void ComponentStateThrow::GUI()
{
	__super::GUI();

	// GUI内に出現させる
	ImGui::Begin(GetOwner()->GetName().data());
	{
		ImGui::Separator();
	}
	ImGui::End();
}

CEREAL_REGISTER_TYPE(ComponentStateThrow)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentStateThrow)
