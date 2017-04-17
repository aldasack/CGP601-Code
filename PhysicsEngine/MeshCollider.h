#pragma once

#ifndef _MESHCOLLIDER_H
#define _MESHCOLLIDER_H

////////////////////////////////
// Name: MeshCollider.h		  //
// Author: Alexander Kocourek //
// Date: 13/04/2017			  //
////////////////////////////////

#include "Collider.h"
#include "Defs.h"

namespace Collision
{
	class MeshCollider :
		public Collider
	{
	public:
		MeshCollider(const RigidBody& rigidbody);
		~MeshCollider();
		void Shutdown();
		void Update() override;
		void AdjustCollider(std::vector<glm::vec3>& vertices);
		std::vector<glm::vec3>& GetVertices() const;
		void SetVertices(std::vector<glm::vec3>& vertices);

	private:
		// Reference to the local coordinates from the GameObject
		std::vector<glm::vec3>* m_pLocalVertices;
		// Transformed coordinates / world coordinates
		std::vector<glm::vec3>* m_pWorldVertices;
	};
}


#endif // !_MESHCOLLIDER_H