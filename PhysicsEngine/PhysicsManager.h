////////////////////////////////
// Name: PhysicsManager.h     //
// Author: Alexander Kocourek //
// Date: 06/02/2017			  //
////////////////////////////////

#pragma once

#ifndef _PHYSICSMANAGER_H
#define _PHYSICSMANAGER_H


#include "Defs.h"

class PhysicsManager
{
public:
	static PhysicsManager& GetInstance()
	{
		if (!s_instance)
			s_instance = new PhysicsManager();

		return *s_instance;
	}
	~PhysicsManager();
	void Shutdown();
	void Update(float deltaTime);
	void AddRigidBody(RigidBody& rigidbody);

private:
	float m_targetFrameTime;
	// stores all RigidBodys to manage them
	std::vector<RigidBody*> m_rigidBodies;

	PhysicsManager();
	bool spheresIntersect(const Collision::SphereCollider &col1, const Collision::SphereCollider &col2);
	bool boxIntersects(const Collision::BoxCollider& col1, const Collision::BoxCollider& col2);
	bool meshIntersects(const Collision::MeshCollider& col1, const Collision::MeshCollider& col2, const glm::quat& rot1, const glm::quat& rot2);
	glm::vec3 support(const glm::vec3& direction, const std::vector<glm::vec3>& points);
	// generates the new direction
	bool doSimplex(std::vector<glm::vec3>& simplex, glm::vec3& direction);

	bool minkowskiSum(const Collision::MeshCollider& col1, const Collision::MeshCollider& col2);
	void collisionResponse(RigidBody& g1, RigidBody& g2);

	static PhysicsManager* s_instance;
};

#endif // !_PHYSICSMANAGER_H_