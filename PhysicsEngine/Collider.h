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
		/*
		*	Creates Collider and pass reference to representing rigidbody
		*/
		Collider(const RigidBody& rigidBody);
		~Collider();
		/*
		*	Destructor replacement.
		*/
		void Shutdown();
		virtual void Update() = 0;

	protected:
		/*
		*	Representing RigidBody
		*/
		const RigidBody* m_pRigidBody;
	};
}

#endif // !_COLLIDER_H