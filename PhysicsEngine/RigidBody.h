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
	//RigidBody(const Math::Vector3 position);
	~RigidBody();
	void Update(float dt);

	Math::Vector3 GetPosition() const;
	void SetPosition(const Math::Vector3 position);

private:
	Math::Vector3 m_position;
	Math::Vector3 m_velocity;
	Math::Vector3 m_acceleration;

	float m_mass;
	Math::Vector3 m_centerOfMass;

	Collider::SphereCollider m_collider;
};

#endif // !_RIGIDBODY_H