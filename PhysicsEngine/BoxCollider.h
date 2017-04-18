#pragma once

////////////////////////////////
// Name: BoxCollider.h	      //
// Author: Alexander Kocourek //
// Date: 27/03/2017			  //
////////////////////////////////

#ifndef _BOXCOLLIDER_H
#define _BOXCOLLIDER_H

#include "Collider.h"

#include "Defs.h"

namespace Collision
{
	class BoxCollider :
		public Collider
	{
	public:
		BoxCollider(RigidBody& rigidbody);
		~BoxCollider();
		void Update() override;
		void AdjustCollider();
		glm::vec3 GetCenter() const;
		std::array<glm::vec3, 3> GetAxes() const;
		glm::vec3 GetExtents() const;

	private:
		glm::vec3 m_center;
		glm::vec3 m_offset;
		std::array<glm::vec3, 3> m_axes;
		glm::vec3 m_extent;
	};
}

#endif // !_BOXCOLLIDER_H