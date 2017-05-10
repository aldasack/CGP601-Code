////////////////////////////////
// Name: PhysicsManager.h     //
// Author: Alexander Kocourek //
// Date: 06/02/2017			  //
////////////////////////////////

#pragma once

#ifndef _PHYSICSMANAGER_H
#define _PHYSICSMANAGER_H


#include "Defs.h"

struct SupportPoint { glm::vec3 v, A, B; };
struct Edge 
{ 
	SupportPoint A, B; 
	Edge(SupportPoint A, SupportPoint B) 
	{
		this->A = A;
		this->B = B;
	}
};
struct Triangle
{
	SupportPoint A, B, C;
	glm::vec3 normal;

	Triangle(SupportPoint A, SupportPoint B, SupportPoint C)
	{
		this->A = A;
		this->B = B;
		this->C = C;
		normal = glm::normalize(glm::cross(B.v - A.v, C.v - A.v));
	}
};

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
	// contact is a out parameter
	bool meshIntersects(RigidBody& rb1, RigidBody& rb2, Collision::ContactData& contact);
	//bool meshIntersects(const Collision::MeshCollider& col1, const Collision::MeshCollider& col2, Collision::ContactData& contact);
	glm::vec3 support(const glm::vec3& direction, const std::vector<glm::vec3>& points);
	// generates the new direction
	bool doSimplex(std::vector<SupportPoint>& simplex, glm::vec3& direction);
	void generateContactData(std::vector<SupportPoint>& simplex, const std::vector<glm::vec3>& A, const std::vector<glm::vec3>& B, Collision::ContactData& contact);
	bool minkowskiSum(const Collision::MeshCollider& col1, const Collision::MeshCollider& col2);
	void collisionResponse(const Collision::ContactData& contact);

	static PhysicsManager* s_instance;
};

#endif // !_PHYSICSMANAGER_H_