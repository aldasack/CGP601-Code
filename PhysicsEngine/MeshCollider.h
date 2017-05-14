////////////////////////////////
// Name: MeshCollider.h		  //
// Author: Alexander Kocourek //
// Date: 13/04/2017			  //
////////////////////////////////

#pragma once

#ifndef _MESHCOLLIDER_H
#define _MESHCOLLIDER_H

#include "Collider.h"
#include "Defs.h"

namespace Collision
{
	class MeshCollider :
		public Collider
	{
	public:
		/*
		*	Creates MeshCollider and pass reference to representing rigidbody
		*/
		MeshCollider(const RigidBody& rigidbody);
		~MeshCollider();
		/*
		*	Destructor replacement.
		*/
		void Shutdown();
		/*
		*	Updates the values of the MeshCollider every frame.
		*/
		void Update() override;
		/*
		*	Sets the Collider values according to vertices, to create a fitting collider.
		*/
		void AdjustCollider(std::vector<glm::vec3>& vertices);
		/*
		*	Returns the transofrmed vertices defining the mesh of the collider.
		*/
		std::vector<glm::vec3>& GetVertices() const;
		/*
		*	Sets the vertices representing the mesh of the collider.
		*/
		void SetVertices(std::vector<glm::vec3>& vertices);

	private:
		/*
		*	Reference to the local coordinates from the GameObject
		*/
		std::vector<glm::vec3>* m_pLocalVertices;
		/*
		*	Transformed coordinates / world coordinates
		*/
		std::vector<glm::vec3>* m_pWorldVertices;
	};
}


#endif // !_MESHCOLLIDER_H