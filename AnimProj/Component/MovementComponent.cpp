// author: Changwan Yu
#include "pch.h"
#include "MovementComponent.h"
#include "../World/World.h"
#include "../Map/GridMap.h"
#include "../Actor/Actor.h"
#include "../Component/SceneComponent.h"
#include "../Component/CombatComponent.h"

namespace pa
{
	void MovementComponent::onUpdate(World& world, Actor& owner, float deltaTime)
	{
		using namespace DirectX;
		if (!owner.isAlive())
			return;

		SceneComponent* sceneComp = owner.getComponent<SceneComponent>();
		assert(sceneComp);

		const XMVECTOR V0	= XMLoadFloat3(&sceneComp->position);
		const XMVECTOR V1	= XMLoadFloat3(&targetPosition);
		const XMVECTOR Vdir = XMVector3Normalize(V1 - V0);

		float distance = XMVectorGetX(XMVector3Length(V1 - V0));
		_isMoving	   = distance > 0.0001f;
		if (!_isMoving)
			return;

		XMVECTOR Forward = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
		Forward			 = XMVector3Transform(Forward, XMMatrixRotationY(sceneComp->eulerAngle.y));

		if (XMVector3Equal(Vdir, Forward))
		{
			_isRotating = false;
		}
		else if (XMVector3Equal(Vdir, -Forward))
		{
			_isRotating = true;
			sceneComp->eulerAngle.y += rotateSpeed * deltaTime;
		}
		else
		{
			const XMVECTOR Axis = XMVector3Cross(Forward, Vdir);
			float		   dot	= XMVectorGetX(XMVector3Dot(Forward, Vdir));
			dot					= std::min(dot, 1.0f);
			dot					= std::max(dot, -1.0f);

			const float axisY = XMVectorGetY(Axis);
			_isRotating		  = (dot != 1.0f);

			if (_isRotating)
			{
				float deltaAngle = std::acosf(dot);
				deltaAngle		 = std::min(deltaAngle, rotateSpeed * deltaTime);

				if (axisY < 0.0f)
					deltaAngle = -1.0f * deltaAngle;

				sceneComp->eulerAngle.y += deltaAngle;
			}
		}

		if (CombatComponent* combatComp = owner.getComponent<CombatComponent>())
		{
			if (combatComp->isAttacking())
			{
				_isMoving = false;
				return;
			}
		}


		distance   = std::min(distance, speed * deltaTime);
		XMVECTOR V = V0 + Vdir * distance;
		XMStoreFloat3(&sceneComp->position, V);
	}
}
