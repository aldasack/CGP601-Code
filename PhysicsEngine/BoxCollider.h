////////////////////////////////
// Name: BoxCollider.h	      //
// Author: Alexander Kocourek //
// Date: 27/03/2017			  //
////////////////////////////////

#pragma once


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
		/*
		*	Creates BoxCollider and pass reference to representing rigidbody
		*/
		BoxCollider(RigidBody& rigidbody);
		~BoxCollider();
		/*
		*	Updates the values of the BoxCollider every frame.
		*/
		void Update() override;
		/*
		*	Sets the Collider values according to vertices, to create a fitting collider.
		*/
		void AdjustCollider();
		/*
		*	Returns center of the BoxCollider.
		*/
		glm::vec3 GetCenter() const;
		/*
		*	Returns axes of the BoxCollider
		*/
		std::array<glm::vec3, 3> GetAxes() const;
		/*
		*	Returns extents of the BoxCollider.
		*/
		glm::vec3 GetExtents() const;

	private:
		/*
		*	Center of the BoxCollider.
		*/
		glm::vec3 m_center;
		/*
		*	Offset of the BoxCollider from GameObject center.
		*/
		glm::vec3 m_offset;
		/*
		*	Axes of the BoxCollider, representing the rotation.
		*/
		std::array<glm::vec3, 3> m_axes;
		/*
		*	Extent of the BoxCollider along each axis.
		*/
		glm::vec3 m_extent;
	};
}

#endif // !_BOXCOLLIDER_H