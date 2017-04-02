#pragma once

////////////////////////////////
// Name: PhysicsManager.h     //
// Author: Alexander Kocourek //
// Date: 06/02/2017			  //
////////////////////////////////

#ifndef _PHYSICSMANAGER_H_
#define _PHYSICSMANAGER_H_

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
	bool boxIntersects(const Collision::BoxCollider & col1, const Collision::BoxCollider & col2);
	void collisionResponse(RigidBody& g1, RigidBody& g2);

	static PhysicsManager* s_instance;
};

#endif // !_PHYSICSMANAGER_H_