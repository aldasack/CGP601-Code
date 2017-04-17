////////////////////////////////
// Name: Collider.h			  //
// Author: Alexander Kocourek //
// Date: 22/03/2017			  //
////////////////////////////////

#pragma once

#ifndef _COLLIDER_H
#define _COLLIDER_H


#include "Defs.h"
#include "RigidBody.h"

namespace Collision
{
	class Collider
	{
	public:
		Collider(const RigidBody& rigidBody);
		~Collider();
		void Shutdown();
		virtual void Update() = 0;
		//virtual void AdjustCollider() = 0;

	protected:
		const RigidBody* m_pRigidBody;

	};
}
#endif // !_COLLIDER_H