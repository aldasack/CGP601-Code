#pragma once

////////////////////////////////
// Name: SphereCollider.h	  //
// Author: Alexander Kocourek //
// Date: 22/03/2017			  //
////////////////////////////////

#ifndef _SPHERECOLLIDER_H
#define _SPHERECOLLIDER_H

#include "Defs.h"
#include "Collider.h"

namespace Collision
{
	class SphereCollider :
		public Collider
	{
	public:
		SphereCollider(RigidBody& rigidBody);
		~SphereCollider();
		void Update() override;
		void AdjustCollider();
		void SetPosition(const glm::vec3& position);
		glm::vec3 GetPosition() const;
		// should be assigned by collider itself
		void SetRadius(const float radius);
		float GetRadius() const;
	
	private:
		glm::vec3 m_position;
		glm::vec3 m_offset;
		float m_radius;
	};

}

#endif // !_SPHERECOLLIDER_H