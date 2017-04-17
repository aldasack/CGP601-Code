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
		BoxCollider(RigidBody& rigidbody/*, const glm::vec3& scale*/);
		~BoxCollider();
		void Update() override;
		void AdjustCollider();
		std::array<glm::vec3, 8> GetEdges() const;
		glm::vec3 GetCenter() const;
		std::array<glm::vec3, 3> GetAxes() const;
		glm::vec3 GetExtents() const;
		// Returns rotation as normalized quaternion
		glm::quat GetRotation() const;

	//private:

		//second approach
		glm::vec3 m_center;
		std::array<glm::vec3, 3> m_axes;
		glm::vec3 m_extent;

	private:
		// first approach
		std::array<glm::vec3, 8> m_edges;
		std::array<glm::vec3, 8> m_transformedEdges;
		glm::quat m_rotation;
		
	};
}

#endif // !_BOXCOLLIDER_H