#pragma once

////////////////////////////////
// Name: Collider.h			  //
// Author: Alexander Kocourek //
// Date: 22/03/2017			  //
////////////////////////////////

#ifndef _COLLIDER_H
#define _COLLIDER_H

#include "Defs.h"
#include "RigidBody.h"

namespace Collision
{
	class Collider
	{
	public:
		Collider(RigidBody& rigidBody);
		~Collider();
		void Shutdown();
		ColliderType GetColliderType() const;
		virtual void Update() = 0;

	protected:
		ColliderType m_colliderType;
		RigidBody* m_pRigidBody;

	};
}
#endif // !_COLLIDER_H