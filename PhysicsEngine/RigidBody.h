#pragma once

#ifndef _RIGIDBODY_H
#define _RIGIDBODY_H

////////////////////////////////
// Name: RigidBody.h		  //
// Author: Alexander Kocourek //
// Date: 06/02/2017			  //
////////////////////////////////

#include <iostream>

#include "Defs.h"

class RigidBody
{
public:
	RigidBody();
	//RigidBody(const glm::vec3 position);
	~RigidBody();
	void Update(float dt);

	glm::vec3 GetPosition() const;
	void SetPosition(const glm::vec3 position);
	//void SetForce(const glm::vec3 force);

	glm::vec3 GetVelocity() const;
	void SetVelocity(const glm::vec3 velocity);
	
	float GetMasss() const;

	bool IsStatic() const;
	void IsStatic(const bool isStatic);

	bool UseGravity() const;
	void UseGravity(const bool useGravity);

	// Adds a force to the rigidbody and calculates the resulting force of all applied forces
	void AddForce(const glm::vec3 force);

private:
	glm::vec3 m_position;
	glm::vec3 m_velocity;
	glm::vec3 m_acceleration;
	glm::vec3 m_force;

	float m_mass;
	glm::vec3 m_centerOfMass;

	bool m_isStatic;
	bool m_useGravity;

	//Collider::SphereCollider m_collider;

	void resetForce();
};

#endif // !_RIGIDBODY_H