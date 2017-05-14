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
	/*
	*	Singleton implementation.
	*	Retruns instance of the PhysicsManager.
	*	If the instance is nullptr, a new PhysicsManager is created
	*/
	static PhysicsManager& GetInstance()
	{
		if (!s_instance)
			s_instance = new PhysicsManager();

		return *s_instance;
	}
	~PhysicsManager();
	/*
	*	Destructor replacement.
	*/
	void Shutdown();
	/*
	*	Called every frame.
	*	Updates all RigidBodies and calls collision detection and collision response mehtods.
	*/
	void Update(float deltaTime);
	/*
	*	Adds a RigidBody, so it gets updated and check for collision every frame.
	*/
	void AddRigidBody(RigidBody& rigidbody);

private:

	/*
	*	Targeted refresh time.
	*/
	float m_targetFrameTime;
	/*
	*	Stores all RigidBodys to manage them.
	*/
	std::vector<RigidBody*> m_rigidBodies;

	PhysicsManager();
	/*
	*	Tests if two SphereCollider, 'col1' and 'col2' intersect.
	*/
	bool spheresIntersect(const Collision::SphereCollider& col1, const Collision::SphereCollider& col2);
	/*
	*	Tests if two BoxCollider 'col1' and 'col2' intersect.
	*/
	bool boxIntersects(const Collision::BoxCollider& col1, const Collision::BoxCollider& col2);
	/*
	*	Tests if two MeshCollider intersect.
	*	'rb1's and 'rb2's MeshCollider are testet for intersection.
	*	'contact' is an outparameter containing the generated contact data.
	*/
	bool meshIntersects(RigidBody& rb1, RigidBody& rb2, Collision::ContactData& contact);
	/*
	*	Support function for GJK algorithm.
	*	Returns furhtest point of 'points' along direction 'direction'.
	*/
	glm::vec3 support(const glm::vec3& direction, const std::vector<glm::vec3>& points);
	/*
	*	Support function for GJK algorithm.
	*	Checks if simplex 'simplex' contains origin.
	*	'direction' is the previous search direction.
	*	Calculates new search direction and is returned out via 'direction'.
	*/
	// generates the new direction
	bool doSimplex(std::vector<SupportPoint>& simplex, glm::vec3& direction);
	/*
	*	Generates contact data of shape 'A' and 'B' with help of a simplex.
	*	'contact' is an outparameter containing the generated contact data.
	*/
	void generateContactData(std::vector<SupportPoint>& simplex, const std::vector<glm::vec3>& A, const std::vector<glm::vec3>& B, Collision::ContactData& contact);
	/*
	*	Calculates the angular and linear velocity after the collision defined by 'contact'
	*/
	void collisionResponse(const Collision::ContactData& contact);

	static PhysicsManager* s_instance;
};

#endif // !_PHYSICSMANAGER_H_